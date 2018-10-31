#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "fitsio.h"
#include "longnam.h"

#define OK 0
#define INVAL -1
#define MAXIDS	256

char lpath[150];
char ldir[128];
char tmpstr[150];
int nextimg = 0;
int abspath = 0;
int ampsperdet=1;
int getid=0;
char *ids[MAXIDS];
int extvalid[MAXIDS];
int nids=0;
extern int xstart, ystart, xlen, ylen;
extern int xoverscan, xprescan;


int DSKIMG_SetListPath (char *path)
{
FILE *lp;

        if ((lp = fopen (path, "r")) == NULL)
                return (-EINVAL);
        strcpy (lpath, path);
        fclose (lp);
	nextimg = 0;
        return (0);
}

int DSKIMG_ParseToken (char *token)
{
int ret=-EINVAL;

	if (token != NULL) {
		if (strcmp (token, "-ampsperdet") == 0){
			token = strtok (NULL, " ");
			ampsperdet = atoi (token);
			ret = 0;
		} else if (strcmp (token, "-extractid") == 0){ 
			getid = 1;
			ret = 0;
		} else
			ret = -EINVAL;
	}
	return (ret);
} 
			

int DSKIMG_SetParams (char *args)
{
char listpath[128];
char listname[128];
char *token;
int ret;

	*listname = '\0';
	sscanf (args, "%s %s", listpath, listname);
	if (*listname == '\0') {
		abspath = 1;
	} else {
		strcat (listpath, "/");
		strcpy (ldir, listpath);
		strcat (listpath, listname);
		abspath = 0;
	}
	printf ("abspath %d, path %s (dir %s)\n", abspath, listpath, ldir);
        token = strtok (args, " ");
        token = strtok (NULL, " ");
        token = strtok (NULL, " ");
	ret = 0;
	getid = 0;
	while (ret == 0){
		if ((ret = DSKIMG_ParseToken (token)) == 0)
			token = strtok (NULL, " ");
	}
	printf ("amps per det %d, extactid %d\n", ampsperdet, getid);
	return (DSKIMG_SetListPath (listpath));
}


int DSKIMG_GetNextName (char *path, char *img)
{
FILE *lp;
int end_of_scan=0;
int cnt=0;

	if ((lp = fopen (path, "r")) == NULL)
		return (-EINVAL);

	while (!end_of_scan){
		if (fgets (img, 80, lp) == NULL) {
			end_of_scan = 1;
			fclose (lp);
			nextimg = 0;
			return (-1);
		}
		if (cnt >= nextimg) {
			img = strtok (img, "\n");
			end_of_scan = 1;
			nextimg++;
			if (!abspath) {
				*tmpstr='\0';
				strcat (tmpstr, ldir);
				strcat (tmpstr, img);
				strcpy (img, tmpstr);
			}				
			printf ("next image (%d) is %s\n", nextimg, img);
			fclose (lp);
			return (cnt);
		}
		cnt++;
	}
	fclose (lp);
	return (0);
}

char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    for (ptr = s + strlen(s) - 1; (ptr >= s); --ptr){
        *ptr = (unsigned char)toupper ((int)*ptr);
    }
    return s;
}
		

int DSKIMG_AddID (char *id)
{
        if (id == NULL)
                return (-EINVAL);
	if (nids >= MAXIDS)
                return (-EDQUOT);

	if (ids[nids] == NULL)
        	ids[nids] = malloc(40);
	trim (id);
        if (strlen (id) > 40)
                strncpy (ids[nids], id, 40);
        else
                strcpy (ids[nids], id);
//        printf ("added ID %s\n", ids[nids]);
        nids++;
        return (OK);
}

void DSKIMG_FreeIDs ()
{
int i;

        if (nids > 0){
                for (i=0; i<nids; i++)
                        free (ids[i]);
        }
        nids=0;
	for (i=0; i<MAXIDS; i++)
		ids[i] = NULL;
}


int DSKIMG_FilterExt (fitsfile *fptr, int index)
{
int status=0;
char name[60];
char *tname;
int i;

        if (nids <= 0)          /*defined?. If not, accept the extension*/
                return (index);

        fits_read_keyword (fptr, "EXTNAME", name , NULL, &status);
        if (status != 0){
                printf ("extension name not found. OK (assuming %d)\n", index);  /*no name, assume is valid*/
                return (index);
        }
/*has name and we have a filter defined. Go filter now*/
        tname = strtok (name, "'");
        trim (tname);
        for (i=0; i<nids; i++){
                if (strcmp (ids[i],tname) == 0){
//                        printf ("name %s (index %d)\n", tname, i);
                        return (i);    /*found it*/
                }
        }
/*never found it. Say it is invalid*/
        return (INVAL);
}



int DSKIMG_GetImage (char *impath, void *buff, int bufsize, int *ncols, int *nrows, int *bpp)
{
fitsfile *fptr;
int status=0;
int bitpix;
int naxis=0;
long naxes[2] = {1,1};
int datatype;
long fpixel[2]={1,1};
long lpixel[2]={0,0};
long inc[2]={1,1};
int nelems;
int xs, ys;
int ret=0;
int hdunum, hdutype, i, dataext, iniext;
void *pbuf;
int validext=0;
int cext=0;

    if (buff == NULL)
	return (-EINVAL);

    printf ("Getting image ...\n");
    fits_open_file (&fptr, impath, READONLY, &status);
    if (status != 0){
	printf ("error %d opening file %s\n", status, impath);
	return (-status);
    }
    fits_get_num_hdus(fptr, &hdunum, &status);
    if (status != 0){
	ret = -status;
	goto bye_img;
    }

    if (hdunum > 1) {
	dataext = hdunum-1;
	iniext = 2;
    } else {
	dataext = 1;
	iniext = 1;
    }
    if (getid){
    	validext = 0;
    	for (i=iniext; i<=hdunum; i++){
        	fits_movabs_hdu (fptr, i, &hdutype, &status);
        	if ((cext = DSKIMG_FilterExt (fptr, i-iniext)) >= 0){
               	 	extvalid[i-iniext]=cext;
             		validext++;
        	} else {
                	extvalid[i-iniext]=INVAL;
		}
	}
    } else {
	for (i=iniext; i<=hdunum; i++)
		extvalid[i-iniext]=i-iniext;
	validext = dataext;
    }
    printf ("image has %d data extensions (%d valid).\n", dataext, validext);
    cext = iniext-1;
    for (i=iniext; i<=hdunum; i++){
	if (iniext > 1)		/*image is mext*/
		fits_movabs_hdu (fptr, i, &hdutype, &status);

	if (extvalid[i-iniext] < 0)
		continue;

    	fits_get_img_param(fptr, 2,  &bitpix, &naxis, naxes, &status);
    	if (status != 0){
		ret = -status;
		goto bye_img;
    	}
	cext++;
	switch (bitpix){
		case SHORT_IMG: 
			datatype = TUSHORT;
			break;
		case USHORT_IMG: 
			datatype = TUSHORT;
			break;
		case LONG_IMG: 
			datatype = TLONG;
			break;
		case ULONG_IMG: 
			datatype = TULONG;
			break;
		case FLOAT_IMG: 
			datatype = TFLOAT;
			break;
		default:
			datatype = bitpix;
	}

    	nelems = naxes[0] * naxes[1];
    	*ncols = naxes[0];
    	*nrows = naxes[1];
    	*bpp = abs(bitpix/8);
	if ((xstart <= 0) || (ystart <=0) || (xlen <=0) || (ylen <=0)) {
		fpixel[0] = 1;
		fpixel[1] = 1;
		lpixel[0] = *ncols;
		lpixel[1] = *nrows;
	} else {
		if ((xs = xstart - xprescan) <= 0)
			xs = 1;
		if ((ys = ystart) <= 0)
			ys = 1;
		fpixel[0] = 1;
		fpixel[1] = 1;
		lpixel[0] = 1 + xlen -1 + xprescan + xoverscan;
		lpixel[1] = 1 + ylen -1;
		nelems = (xlen + xprescan + xoverscan) * (ylen);
	} 
//	pbuf = buff + (cext-iniext)*nelems*(*bpp);
	pbuf = buff + extvalid[i-iniext]*nelems*(*bpp);
//        printf ("%d: ncols %d, nrows %d, (bitpix %d, datatype %d), coords: (%ld, %ld), (%ld, %ld) xshift %d yshift %d\n", cext, *ncols, *nrows, bitpix, datatype, fpixel[0], fpixel[1], lpixel[0], lpixel[1], xshift, yshift);
    	if ((pbuf != NULL) && (bufsize >= (validext*nelems*(*bpp)))) {
		if (getid)
			printf ("copying pixels from ext %d (%s)\n", i, ids[extvalid[i-iniext]]);
        //  	fits_read_pix (fptr, datatype, fpixel, nelems , NULL , pbuf, NULL, &status);
		status = 0;
          	fits_read_subset (fptr, datatype, fpixel, lpixel, inc, NULL , pbuf, NULL, &status);
//		status = 0;
    		if (status != 0){
			ret = -status;
			printf ("****ERROR %d copying pixels****\n", ret);
			goto bye_img;
    		}
    	} else {
		ret=-ENOMEM;
		printf ("****ERROR buffer too small or null (have %d, required %d)****\n", bufsize, validext*nelems*(*bpp));
		goto bye_img;
	}
    }

bye_img:	
    fits_close_file (fptr, &status);
    return (ret);
}

int DSKIMG_GetPixels (void *buff, int bufsize, int *ncols, int *nrows, int *bpp)
{
int ret;
char img[80];

	if ((ret = DSKIMG_GetNextName (lpath, img))<0){
		if ((ret = DSKIMG_GetNextName (lpath, img))<0)
			return (-EINVAL);
	}
        ret = DSKIMG_GetImage (img, buff, bufsize, ncols, nrows, bpp);
        return (ret);
}


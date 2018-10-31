int DSKIMG_GetImage (char *impath, void *buff, int bufsize, int *ncols, int *nrows, int *bpp);
int DSKIMG_GetPixels (void *buff, int bufsize, int *ncols, int *nrows, int *bpp);
int DSKIMG_SetListPath (char *path);
int DSKIMG_SetParams (char *args);
int DSKIMG_AddID (char *id);
void DSKIMG_FreeIDs ();

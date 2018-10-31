/* Math expression parser/unparser/evaluator */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("MATHP: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif


struct node
 {
 int what;		/* What this node is */
 double value;		/* Value of number (or variable?) */
 char name[32];		/* Name of variable */
 struct node *right;	/* Right side of operator */
 struct node *left;	/* Left side of operator */
 int swaps;
 };

/* Node types */

#define wNUM	1	/* Number */
#define wVAR	2	/* Variable */
#define wFUNC	3	/* Function */

/* Precidence 40 */
#define wEXP	4

/* Precidence 30 */
#define wNEG	5

/* Precidence 20 */
#define wMUL	6
#define wDIV	7

/* Precidence 10 */
#define wADD	8
#define wSUB	9

/* Input string pointer */
char *str;

/* Error flag.  Is set if the expression is incomplete */
static int err;

struct node *doparse(prec)
{
struct node *node=(struct node *)calloc(sizeof(struct node),1);
struct node *op;
int opprec, assoc;

/* Skip whitespace */
while(*str==' ' || *str=='\t') ++str;

if(*str>='0' && *str<='9' || *str=='.')
 {
 /* Found a number */
 node->what=wNUM;
 sscanf(str,"%lf",&(node->value));
 while(*str>='0' && *str<='9' || *str=='.' || *str=='e' || *str=='E') ++str;
 }
else if(*str>='A' && *str<='Z' || *str>='a' && *str<='z' || *str=='_')
 {
 /* Found a name */
 int x;
 node->what=wVAR;
 for(x=0;str[x]>='A' && str[x]<='Z' || str[x]>='a' && str[x]<='z' ||
         str[x]>='0' && str[x]<='9' || str[x]=='_';++x)
  node->name[x]=str[x];
 node->name[x]=0;
 str+=x;
 
 /* Check if this is a function call */
 while(*str==' ' || *str=='\t') ++str;
 if(*str=='(')
  {
  ++str;
  node->what=wFUNC;
  node->right=doparse(0);
  while(*str==' ' || *str=='\t') ++str;
  if(*str==')') ++str;
  else printf("MATHP: Missing right parenthasis\n");
  }
 }
else if(*str=='(')
 {
 /* Found a parenthasis */
 free(node);
 ++str;
 node=doparse(0);
 while(*str==' ' || *str=='\t') ++str;
 if(*str==')') ++str;
 else printf("MATHP: Missing right parenthasis\n");
 }
else if(*str=='-')
 {
 ++str;
 node->what=wNEG;
 node->right=doparse(30);
 }
else
 {
 err=1;
 printf("MATHP: Missing expression\n");
 free(node);
 return 0;
 }

loop:
while(*str==' ' || *str=='\t') ++str;
op=(struct node *)calloc(sizeof(struct node),1);
assoc=0;

switch(*str)
 {
 case '^': opprec=40; op->what=wEXP; assoc=1; break;
 case '*': opprec=20; op->what=wMUL; break;
 case '/': opprec=20; op->what=wDIV; break;
 case '+': opprec=10; op->what=wADD; break;
 case '-': opprec=10; op->what=wSUB; break;
 default: free(op); return node;
 }

if(opprec>prec || assoc && opprec>=prec)
 {
 ++str;
 op->left=node;
 op->right=doparse(opprec);
 node=op;
 goto loop;
 }

free(op);
return node;
}

struct node *parse(char *s)
{
str=s;
err=0;
return doparse(0);
}

static double var;

double ev( struct node *node)
{

//printf ("in ev: %d\n", node->what);
switch(node->what)
 {
case wNUM: return node->value;
case wEXP: return pow(ev(node->left),ev(node->right));
case wMUL: return ev(node->left)*ev(node->right);
case wDIV: return ev(node->left)/ev(node->right);
case wADD: return ev(node->left)+ev(node->right);
case wSUB: return ev(node->left)-ev(node->right);
case wNEG: return -ev(node->right);
case wVAR: /* Variable lookup */
           if(!strcmp(node->name,"x")) return var;
           else
            {
            printf("MATHP: Unknown variable\n");
            return 0.0;
            }
case wFUNC: /* Function call lookup */
           if(!strcmp(node->name,"sin")) return sin(ev(node->right));
           else if(!strcmp(node->name,"cos")) return cos(ev(node->right));
           else if(!strcmp(node->name,"tan")) return tan(ev(node->right));
           else
            {
            printf("MATHP: Unknown function\n");
            return 0.0;
            }
case 0: return 0.0;
 }
 return (0.0);
}

void rmnode(struct node* node)
{
if(node)
 {
 rmnode(node->left);
 rmnode(node->right);
 free(node);
 }
}

int MP_evaluate (double variable, char *expression, double *resp)
{
struct node *node;
char buf[128];

 if (strlen (expression) <=0)
	return (-EINVAL);
 
pdebug ("MP_eval: expression: %s, with x=%g\n", expression, variable);
 strcpy (buf, expression);
 node=parse(buf);
 var = variable;
 *resp = ev (node);
pdebug ("MP_eval: Answer is: %g\n",*resp);
 if (err)
	printf ("MATHP: there was an error\n");
 rmnode (node);
 return (err);
}

/*
 * ide.h
 *
 * Data structures used by the Interactive Diagnostic Environment
 * interpreter.
 *
 * $Revision: 1.1.1.1 $
 */

#include "setjmp.h"
#include "ctype.h"
#ifdef STANDALONE
#include "saioctl.h"
#endif

/* all statements are parsed into binary trees by ide_gram.y.  The tree
   is made up of nodes of the following form: */

typedef struct node_s {
	int node_type;			/* type, defined below */
	struct node_s *node_right;	/* right child, zero if none */
	union	{
		struct node_s *_left;
		struct sym_s *_psym;
		} _val;
#define node_left _val._left		/* left child if another node */
#define node_psym _val._psym		/* left child sometimes a symbol */
	} node_t;

/* symbol structure */
typedef struct sym_s {
	struct sym_s *sym_hlink;	/* hash link */
	union	{
		char	*_name;
		int	_indx;
		} _tag;
#define sym_name _tag._name		/* pointer to symbol name */
#define sym_indx _tag._indx		/* index of parameter (e.g. $1, $2) */
	int	sym_basetype;		/* data type: int, string, etc. */
	int	sym_type;		/* symbol type */
	int	sym_flags;
	union	{
		int	_i;
		char	*_s;
		int     *_a;
		int	((*_func)());
		node_t	*_stmt;
		struct	{
			int _start;
			int _count;
			} _range;
		} _val;
#define sym_i _val._i
#define sym_s _val._s
#define sym_a _val._a
#define sym_func  _val._func
#define sym_stmt  _val._stmt
#define sym_start _val._range._start
#define sym_count _val._range._count
	} sym_t;

/* symbol base types */
#define SYM_INT		1
#define SYM_STR		2
#define SYM_RANGE	3

/* symbol types */
#define SYM_UNDEFINED	0
#define SYM_VAR		1
#define SYM_CON		2
#define SYM_CMD		3
#define SYM_CMDFLAG	4
#define SYM_PARAM	5
#define SYM_GLOBAL	6

/* symbol flags */
#define SYM_INTABLE	1 /* symbol is in symbol table */
#define SYM_UDEF	2 /* user defined function */
#define SYM_CHARARGV	4 /* pass arguments in "char *argv[]" format */
#define SYM_DIAGCOMM	8 /* pass arguments in using a diag comm struct. */

/* program limits and tunables */
#define HASHSIZE	101	/* should be prime */
#define MAXIDENT	128	/* maximum identifier length */
#define MAXSTRLEN	2048	/* maximum string length */
#define MAXARGC		64	/* maximum number of passed arguments */
#define MAXLINELEN	2048

#ifndef EOF
#define EOF (-1)
#endif

/* node types */
#define OP_VAR		1
#define OP_ADD		2
#define OP_SUB		3
#define OP_UMINUS	4
#define OP_MUL		5
#define OP_DIV		6
#define OP_MOD		7
#define OP_OR		8
#define OP_AND		9
#define OP_XOR		10
#define OP_COM		11
#define OP_NOT		12
#define OP_ANDAND	13
#define OP_OROR		14
#define OP_LT		15
#define OP_LE		16
#define OP_GT		17
#define OP_GE		18
#define OP_EQ		19
#define OP_NE		20
#define OP_PRED		21
#define OP_PREI		22
#define OP_POSTD	23
#define OP_POSTI	24
#define OP_SHL		25
#define OP_SHR		26
#define OP_ASSIGN	27
#define OP_SUBASS	29
#define OP_ADDASS	28
#define OP_MULASS	30
#define OP_DIVASS	31
#define OP_MODASS	32
#define OP_SHLASS	33
#define OP_SHRASS	34
#define OP_ANDASS	35
#define OP_ORASS	36
#define OP_XORASS	37

#define OP_RETURN	38
#define OP_CONTINUE	39
#define OP_BREAK	40
#define OP_DO		41
#define OP_SWITCH	42
#define OP_CASELIST	43
#define OP_CASE		44
#define OP_CMD		45
#define OP_STMT		46
#define OP_IF		47
#define OP_ELSE		48
#define OP_FCALL	49
#define OP_ARGLIST	50
#define OP_PRINT	51
#define OP_WHILE	52
#define OP_FOR		53
#define OP_REPEAT	54

/* execution context frame */
typedef struct context_s {
	jmp_buf lframe; /* loop frame */
	jmp_buf rframe; /* return frame */
	sym_t	*argv[MAXARGC];
	int	argc;
	} context_t;

extern node_t	*makenode();
extern char	*makestr();
extern char	*makecatstring();
extern sym_t	*makesym();
extern sym_t	*makeisym();
extern sym_t	*makessym();
extern sym_t	*findsym();
extern sym_t	*eval();
extern char 	*talloc();
extern sym_t	*pro_int();
extern sym_t	*pro_str();
extern char	*getenv();

#define DIV0RESULT 0x7fffffff /* result of divide or modulo by zero */

/* structure used for initializing symbols (can't initialize unions!) */
typedef struct builtin_s {
	char	*name;
	char	*addr;
	int	type;
	char	*help;
	} builtin_t;

#define B_CMD 1
#define B_STR 2
#define B_INT 3
#define B_SCMD 4
#define B_DCMD 5

/* puts() behaves differently in standalone mode */

#ifdef STANDALONE
#define ide_puts(t) puts(t)
#else
#define ide_puts(x) printf("%s", x)
#endif

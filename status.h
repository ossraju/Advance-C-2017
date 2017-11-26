#ifndef STATUS_H
#define STATUS_H
typedef enum {
	OK,
	ERROPEN,
	ERRACCESS,
	ERRCLOSE,
	ERRALLOC,
	ERRDATE,
	ERRFULL,
	ERREMPTY,
	ERRABSENT,
	ERREXIST,
	ERRINDEX,
	ERRUNABLE,

	ERRUNKNOWN,
	} status;

	extern char* message(status s);
#endif // STATUS_H



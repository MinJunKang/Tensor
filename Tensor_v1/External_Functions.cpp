#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <locale>

#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <Windows.h>
#ifdef _WIN64
   //define something for Windows (64-bit only)
#include <Windows.h>
#endif
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#   error "Unknown Apple platform"
#endif
#elif __linux__
// linux
#include <sys/types.h>
#elif __unix__ // all unices not caught above
// Unix
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <termios.h>
#include <fcntl.h>

int terminal_specs(int *const cols, int *const rows,
	int *const col, int *const row)
{
	int	 	in, out, result;
	char		buffer[16];
	struct winsize	ws;
	struct termios	t1, t2;

	/* Initialize zeros (unknown) */
	if (cols) *cols = 0;
	if (rows) *rows = 0;
	if (col)  *col = 0;
	if (row)  *row = 0;

	/* Open controlling tty for input. */
	do {
		in = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	} while (in == -1 && errno == EINTR);
	if (in == -1)
		return errno;

	/* Reopen TTY for output. */
	do {
		out = open("/dev/tty", O_WRONLY);
	} while (out == -1 && errno == EINTR);
	if (out == -1) {
		int const saved_errno = errno;
		do {
			result = close(in);
		} while (result == -1 && errno == EINTR);
		return saved_errno;
	}

	/* Determine terminal size. */
	if (!ioctl(out, TIOCGWINSZ, &ws)) {
		if (cols) *cols = ws.ws_col;
		if (rows) *rows = ws.ws_row;
	}

	/* Save current terminal settings. */
	tcgetattr(out, &t1);
	tcgetattr(out, &t2);

	/* Disable echo and signals for now. */
	t2.c_iflag &= ~(IGNBRK | IGNPAR);
	t2.c_oflag &= ~(OFILL);
	t2.c_oflag |= ONOCR;
	t2.c_lflag &= ~(ISIG | ICANON | ECHO);
	tcsetattr(out, TCSANOW, &t2);

	/* Query cursor location. */
	if (row || col) {

		if (write(out, "\033[6n", 4) == 4) {
			do {
				result = read(in, buffer, sizeof(buffer) - 1);
			} while (result == -1 && (errno == EINTR || errno == EWOULDBLOCK));
			if (result > 0 && buffer[result - 1] == 'R') {
				char *p = buffer;
				int  nr = 0;
				int  nc = 0;

				buffer[result - 1] = 0;

				/* parse [^0-9]*([0-9]*)[^0-9]*([0-9]*) */
				while (*p && !(*p >= '0' && *p <= '9')) p++;
				while (*p >= '0' && *p <= '9')
					nr = nr * 10 + (*(p++) - '0');
				while (*p && !(*p >= '0' && *p <= '9')) p++;
				while (*p >= '0' && *p <= '9')
					nc = nc * 10 + (*(p++) - '0');

				if (row) *row = nr;
				if (col) *col = nc;
			}
		}
	}

	/* Restore terminal settings. */
	tcsetattr(out, TCSANOW, &t1);

	/* Close descriptors. */
	do {
		result = close(in);
	} while (result == -1 && errno == EINTR);
	do {
		result = close(out);
	} while (result == -1 && errno == EINTR);

	return 0;
}




#elif defined(_POSIX_VERSION)
// POSIX
#else
#   error "Unknown compiler"
#endif

using namespace std;

/************************************************ Function Definition  ***************************************************/

/* Function defined for I/O */
void clear_screen();
void gotoxy(int x, int y);

/* For Error Reporting */
void Print_Error(wstring loc, wstring message, bool exits);
void Make_Error_log();
void End_Error_log();
void Setting();
void Ending();

/* Basic str function */
string To_str(const wstring & wstr);
wstring To_wstr(const string & str);

/*************************************************************************************************************************/



/*************************************************** I/O functions  ******************************************************/


void clear_screen()
{
#ifdef WIN32
	std::system("cls");
#else
	// Assume POSIX
	std::system("clear");
#endif
}

void gotoxy(int x, int y) {
	
#ifdef WIN32
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
#else
	// Assume POSIX
	printf("\033[%d;%df", y, x);
	fflush(stdout);
#endif
}

short wherex()
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
#else
	// Assume POSIX
	int  cols, rows, col, row;
	terminal_specs(&cols, &rows, &col, &row);
	return col;
#endif
}

short wherey()
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.Y;
#else
	// Assume POSIX
	int  cols, rows, col, row;
	terminal_specs(&cols, &rows, &col, &row);
	return row;
#endif
}


/*************************************************************************************************************************/

/*************************************************** Error Handles  ******************************************************/


void Setting() {
	/* set locale version to kor */

	setlocale(LC_ALL, "korean");
	wcout.imbue(locale("korean"));
	wcin.imbue(locale("korean"));

	Make_Error_log();
}

void Ending() {
	End_Error_log();
}

/* Generate Error log */
void Make_Error_log()
{
	FILE * fp;
	fp = fopen("Error_log.txt", "wt");

	if (fp) {
		fwprintf(fp, L"[ 작성하시지 않으시려면 '종료'를 눌러주세요 ]\n\n");
		fwprintf(fp, L"* 이 내용은 개발자에게 e-mail로 전송됩니다.\n\n");
		fwprintf(fp, L"* Easy_Dic v2.0 Log\n----------------------\n");

		fclose(fp);
	}
}

/* End the error log & Execute program */
void End_Error_log()
{
	FILE * fp;
	fp = fopen("Error_log.txt", "at");
	if (fp) {

		fwprintf(fp, L"\n----------------------\n");
		fwprintf(fp, L"\n* User Opinion(개발에 참고가 됩니다. 자유롭게 작성해주세요 :) : \n");
		fclose(fp);
	}
}

/* Error printing */
void Print_Error(wstring loc, wstring message, bool exits = true)
{
	FILE * fp;

	fp = fopen("Error_log.txt", "at");

	if (fp) {
		if (exits) {
			clear_screen();
			cout << "Error happened at class = ";
			wcout << loc.c_str() << endl;
			wcout << L"Point = " << message.c_str() << endl;
			fwprintf(fp, L"\nError : Loc = %s, Code = %s \n", loc.c_str(), message.c_str());
		}
		else
			fwprintf(fp, L"\nWarning : Loc = %s, Code = %s \n", loc.c_str(), message.c_str());
		fclose(fp);
	}

	if (exits) {
		End_Error_log();
		exit(1);
	}
}


/*************************************************************************************************************************/

/************************************************* Basic str function  ***************************************************/

wstring To_wstr(const string & str)
{
	unsigned int required_len = str.length();

	wchar_t * dest = new wchar_t[required_len + 1];

	mbstowcs(dest, str.c_str(), required_len);

	dest[required_len] = L'\0';

	wstring converted_str(dest);

	delete[] dest;

	return converted_str;
}

string To_str(const wstring & wstr)
{
	unsigned int required_len = 0;
	wchar_t buf[2];
	char dest_buf[2];
	buf[1] = L'\0';

	for (int i = 0; i < wstr.length(); i++) {
		buf[0] = wstr[i];
		wcstombs(dest_buf, buf, 2);
		if (dest_buf[0] & 0x80)
			required_len += 2;
		else
			required_len += 1;
	}

	char * dest = new char[required_len + 1];

	wcstombs(dest, wstr.c_str(), required_len);

	dest[required_len] = '\0';

	string converted_str(dest);

	delete[] dest;

	return converted_str;
}

/*************************************************************************************************************************/
/* test_file_io_lib.c - exercises file_io_lib (2026). Run with `make test`. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "str_lib.h"
#include "log_lib.h"
#include "file_io_lib.h"
#include "file_io_lib_version.h"

static int n_pass, n_fail;
#define CHECK(c) do { if (c) { n_pass++; printf("  ok   %s\n", #c); } \
	else { n_fail++; printf("  FAIL %s  (%s:%d)\n", #c, __FILE__, __LINE__); } } while (0)
#define DONE(name) do { printf("%s: %d passed, %d failed\n", name, n_pass, n_fail); \
	return n_fail ? 1 : 0; } while (0)

#define TEST_DIR  "test_file_io_lib.dir"
#define TEST_FILE TEST_DIR "/data.txt"

int main(void)
{
	char home[PATH_SIZE];
	char name[] = "/usr/local/bin/tool";
	path p;
	FILE *f;

	printf("file_io_lib %s (%s)\n", FILE_IO_LIB_VERSION,
	       FILE_IO_LIB_ORIGIN);
	log_lib_use_stdout(TRUE);

	setenv("HOME", "/tmp/fiotest-home", 1);
	CHECK(get_home_dir(home) == (int) strlen("/tmp/fiotest-home"));
	CHECK(strcmp(home, "/tmp/fiotest-home") == 0);

	/* 1.1 fix: with HOME unset it called strlen(NULL) and crashed */
	{
		struct passwd *pw = getpwuid(getuid());
		const char *want = (pw && pw->pw_dir) ? pw->pw_dir : "";

		unsetenv("HOME");
		CHECK(get_home_dir(home) == (int) strlen(want));
		CHECK(strcmp(home, want) == 0);
		setenv("HOME", "/tmp/fiotest-home", 1);
	}

	CHECK(strcmp(niobasename(name), "tool") == 0);
	CHECK(strcmp(niobasename("plain"), "plain") == 0);

	rmdir(TEST_DIR);
	check_dir(TEST_DIR);
	CHECK(access(TEST_DIR, F_OK) == 0);
	CHECK(file_exists(TEST_DIR) == FALSE);	/* a dir is not a file */
	check_dir(TEST_DIR);			/* second call is a no-op */

	CHECK(file_exists(TEST_FILE) == FALSE);
	f = fopen(TEST_FILE, "w");
	CHECK(f != NULL);
	if (f != NULL) {
		fputs("0123456789", f);
		fclose(f);
	}
	CHECK(file_exists(TEST_FILE) == TRUE);
	CHECK(file_size(TEST_FILE) == 10);
	CHECK(file_size(TEST_DIR) == 0);

	CHECK(pathcpy(p, "/home/") == 6);
	CHECK(pathcat(p, "chris") == 11 && strcmp(p, "/home/chris") == 0);

	remove(TEST_FILE);
	rmdir(TEST_DIR);

	DONE("file_io_lib");
}

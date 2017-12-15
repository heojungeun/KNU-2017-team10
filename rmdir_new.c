#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

static int rmdir_plus(const char *name);

int rmdirnew(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Syntax: %s <dir>\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (rmdir_plus(argv[1]) == 0) {
		return EXIT_SUCCESS;
	}
	else {
		perror("rmdir_plus");
		return EXIT_FAILURE;
	}
}

int rmdir_plus(const char *name) {
	DIR *dir;
	struct dirent *dirent;
	char path[PATH_MAX];

	if (rmdir(name) < 0) {
		switch (errno) {
		case ENOTDIR:
			return unlink(name);
		case ENOTEMPTY:
			// Erase content

			dir = opendir(name);

			if (!dir) {
				return -1;
			}

			while (dirent = readdir(dir), dirent) {
				// Skip "." and ".."
				if (dirent->d_name[0] == '.' && (dirent->d_name[1] == '\0' || (dirent->d_name[1] == '.' && dirent->d_name[2] == '\0'))) {
					continue;
				}

				if (snprintf(path, PATH_MAX, "%s/%s", name, dirent->d_name) >= PATH_MAX) {
					closedir(dir);
					return -1;
				}

				if (rmdir_plus(path) < 0) {
					closedir(dir);
					return -1;
				}
			}

			closedir(dir);

			// Try to erase again

			if (rmdir(name) < 0) {
				return -1;
			}

			break;

		default:
			return -1;
		}
	}

	return 0;
}

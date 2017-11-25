#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

int rmdir_ex(const char *name) {
	DIR *dir;
	struct dirent *dirent;
	char path[PATH_MAX];

	if (rmdir(name) < 0) {
		switch (errno) {
		case ENOTDIR: /// not a directory
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

				if (rmdir_ex(path) < 0) {
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
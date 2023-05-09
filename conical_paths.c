#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

int main()
{
    char* rootdir = "/home/user/docroot/";
    char* requested_resource = "/../non_existing_directory/file";
    char root_path[PATH_MAX];
    char requested_path[PATH_MAX];
    char* canonical_root_path;
    char* canonical_requested_path;

    // Get the canonical path of the document root
    canonical_root_path = realpath(rootdir, root_path);
    if (canonical_root_path == NULL) {
        fprintf(stderr, "Error getting canonical path of document root: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Get the canonical path of the requested resource
    canonical_requested_path = realpath(requested_resource, requested_path);
    if (canonical_requested_path == NULL) {
        if (errno == ENOENT) {
            printf("Requested resource does not exist\n");
        } else {
            fprintf(stderr, "Error getting canonical path of requested resource: %s\n", strerror(errno));
        }
        exit(EXIT_FAILURE);
    }

    // Compare the canonical paths
    if (strncmp(canonical_requested_path, canonical_root_path, strlen(canonical_root_path)) == 0) {
        printf("Requested resource is within document root\n");
    } else {
        printf("Requested resource is outside of document root\n");
    }

    exit(EXIT_SUCCESS);
}

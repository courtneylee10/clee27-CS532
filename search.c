/*
Name: Courtney Lee
BlazerId: clee27
Project #: Homework 2
To compile: make
To run: ./search
*/

#define _XOPEN_SOURCE 700
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    bool show_details;       /* -S */
    bool has_size_limit;     /* -s SIZE */
    off_t size_limit;
    bool has_pattern;        /* -f PATTERN DEPTH */
    char *pattern;
    long max_depth;
    int type_filter;         /* 0 = any, 'f' = regular files, 'd' = directories */
} SearchOptions;

typedef bool (*EntryPredicate)(const char *, const struct stat *, long,
                               const SearchOptions *);

static bool size_matches(const char *path, const struct stat *st, long depth,
                         const SearchOptions *opts);
static bool pattern_matches(const char *path, const struct stat *st, long depth,
                            const SearchOptions *opts);
static bool type_matches(const char *path, const struct stat *st, long depth,
                         const SearchOptions *opts);

static EntryPredicate predicates[] = {
    size_matches,
    pattern_matches,
    type_matches
};

static void print_permissions(mode_t mode)
{
    char p[11] = "----------";
    if (S_ISDIR(mode)) p[0] = 'd';
    else if (S_ISLNK(mode)) p[0] = 'l';
    else if (S_ISCHR(mode)) p[0] = 'c';
    else if (S_ISBLK(mode)) p[0] = 'b';
    else if (S_ISFIFO(mode)) p[0] = 'p';
    else if (S_ISSOCK(mode)) p[0] = 's';

    if (mode & S_IRUSR) p[1] = 'r';
    if (mode & S_IWUSR) p[2] = 'w';
    if (mode & S_IXUSR) p[3] = 'x';
    if (mode & S_IRGRP) p[4] = 'r';
    if (mode & S_IWGRP) p[5] = 'w';
    if (mode & S_IXGRP) p[6] = 'x';
    if (mode & S_IROTH) p[7] = 'r';
    if (mode & S_IWOTH) p[8] = 'w';
    if (mode & S_IXOTH) p[9] = 'x';

    printf("%s", p);
}

static void print_entry(const char *path, const char *name,
                        const struct stat *st, long depth,
                        const SearchOptions *opts)
{
    for (long i = 0; i < depth; ++i) printf("\t");

    if (opts->show_details) {
        printf("%s (size: %lld, permissions: ", name,
               (long long)(S_ISDIR(st->st_mode) ? 0 : st->st_size));
        print_permissions(st->st_mode);
        char timebuf[64];
        struct tm *tm_info = localtime(&st->st_atime);
        if (tm_info != NULL &&
            strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_info) > 0) {
            printf(", last access: %s", timebuf);
        } else {
            printf(", last access: unknown");
        }
        printf(")");
    } else {
        printf("%s", name);
    }

    if (S_ISLNK(st->st_mode)) {
        char target[PATH_MAX + 1];
        ssize_t n = readlink(path, target, sizeof(target) - 1);
        if (n >= 0) {
            target[n] = '\0';
            printf(" (%s)", target);
        }
    }
    putchar('\n');
}

static bool size_matches(const char *path, const struct stat *st, long depth,
                         const SearchOptions *opts)
{
    (void)path; (void)depth;
    return !opts->has_size_limit || st->st_size <= opts->size_limit;
}

static bool pattern_matches(const char *path, const struct stat *st, long depth,
                            const SearchOptions *opts)
{
    (void)path; (void)st;
    return !opts->has_pattern ||
           (depth <= opts->max_depth && strstr(path, opts->pattern) != NULL);
}

static bool type_matches(const char *path, const struct stat *st, long depth,
                         const SearchOptions *opts)
{
    (void)path; (void)depth;
    if (opts->type_filter == 0) return true;
    if (opts->type_filter == 'f') return S_ISREG(st->st_mode);
    if (opts->type_filter == 'd') return S_ISDIR(st->st_mode);
    return true;
}

static bool entry_matches(const char *path, const struct stat *st, long depth,
                          const SearchOptions *opts)
{
    for (size_t i = 0; i < sizeof(predicates) / sizeof(predicates[0]); ++i) {
        if (!predicates[i](path, st, depth, opts)) return false;
    }
    return true;
}

static void traverse(const char *directory, long depth, const char *display_name,
                    const SearchOptions *opts)
{
    struct stat st;
    if (lstat(directory, &st) != 0) {
        fprintf(stderr, "search: can't access '%s': %s\n",
                directory, strerror(errno));
        return;
    }

    if (entry_matches(directory, &st, depth, opts)) {
        print_entry(directory, display_name, &st, depth, opts);
    }

    if (!S_ISDIR(st.st_mode)) return;

    DIR *dir = opendir(directory);
    if (dir == NULL) {
        fprintf(stderr, "search: can't open directory '%s': %s\n",
                directory, strerror(errno));
        return;
    }

    struct dirent *item;
    while ((item = readdir(dir)) != NULL) {
        if (strcmp(item->d_name, ".") == 0 ||
            strcmp(item->d_name, "..") == 0) {
            continue;
        }

        char child[PATH_MAX];
        int written;
        if (strcmp(directory, "/") == 0) {
            written = snprintf(child, sizeof(child), "/%s", item->d_name);
        } else {
            written = snprintf(child, sizeof(child), "%s/%s",
                               directory, item->d_name);
        }
        if (written < 0 || (size_t)written >= sizeof(child)) {
            fprintf(stderr, "search: path too long;'%s/%s'\n",
                    directory, item->d_name);
            continue;
        }

        traverse(child, depth + 1, item->d_name, opts);
    }

    closedir(dir);
}

static void usage(const char *program)
{
    fprintf(stderr,
        "Usage: %s [-S] [-s SIZE] [-f PATTERN DEPTH] [-t f|d] [DIRECTORY]\n"
        "  -S             Show size, permissions, and last access time\n"
        "  -s SIZE        Include entries whose size is <= SIZE bytes\n"
        "  -f PATTERN DEPTH  Match names containing PATTERN at depth <= DEPTH\n"
        "  -t f           Show regular files only\n"
        "  -t d           Show directories only\n",
        program);
}

static bool parse_nonnegative_long(const char *text, long *value)
{
    char *end = NULL;
    errno = 0;
    long parsed = strtol(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' || parsed < 0) return false;
    *value = parsed;
    return true;
}

int main(int argc, char *argv[])
{
    SearchOptions opts = {0};
    int option;
    opterr = 0;

    /*
     * GNU/POSIX getopt parses -S, -s, -f, and -t.
     */
    while ((option = getopt(argc, argv, "Ss:f:t:")) != -1) {
        switch (option) {
        case 'S':
            opts.show_details = true;
            break;
        case 's': {
            long size_value;
            if (!parse_nonnegative_long(optarg, &size_value)) {
                fprintf(stderr, "search: SIZE must be a nonnegative integer\n");
                usage(argv[0]);
                return EXIT_FAILURE;
            }
            opts.has_size_limit = true;
            opts.size_limit = (off_t)size_value;
            break;
        }
        case 'f': {
            long depth_value;
            if (optind >= argc || !parse_nonnegative_long(argv[optind], &depth_value)) {
                fprintf(stderr, "search: -f requires PATTERN followed by a nonnegative DEPTH\n");
                usage(argv[0]);
                return EXIT_FAILURE;
            }
            opts.has_pattern = true;
            opts.pattern = optarg;
            opts.max_depth = depth_value;
            ++optind;
            break;
        }
        case 't':
            if (strcmp(optarg, "f") == 0) opts.type_filter = 'f';
            else if (strcmp(optarg, "d") == 0) opts.type_filter = 'd';
            else {
                fprintf(stderr, "search: -t must be followed by 'f' or 'd'\n");
                usage(argv[0]);
                return EXIT_FAILURE;
            }
            break;
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (argc - optind > 1) {
        fprintf(stderr, "search: there can only be one starting directory supplied\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *start = (optind < argc) ? argv[optind] : ".";
    struct stat start_stat;
    if (lstat(start, &start_stat) != 0) {
        fprintf(stderr, "search: can't access '%s': %s\n", start, strerror(errno));
        return EXIT_FAILURE;
    }

    const char *root_name = start;
    if (strcmp(start, ".") == 0) root_name = ".";
    traverse(start, 0, root_name, &opts);
    return EXIT_SUCCESS;
}

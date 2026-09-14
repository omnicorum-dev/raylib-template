#define NOB_IMPLEMENTATION
#include "nob.h"
#define FLAG_IMPLEMENTATION
#include "flag.h"

Cmd          cmd    = {};
static Cc_Db compdb = {0};

static void usage(void) {
    fprintf(stderr,
            "Usage: %s [<FLAGS>] [--] [<program args>]\n",
            flag_program_name());
    fprintf(stderr, "FLAGS:\n");
    flag_print_options(stderr);
}

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);

    bool run  = false;
    bool help = false;
    flag_bool_var(&run, "run", false, "Run the program after compilation");
    flag_bool_var(&help, "help", false, "Print this help message");

    if (!flag_parse(argc, argv)) {
        usage();
        flag_print_error(stderr);
        return 1;
    }

    if (help) {
        usage();
        return 0;
    }

    cmd_append(&cmd, "cc");
    // cmd_append(&cmd, "-Wall");
    // cmd_append(&cmd, "-Wextra");

    cmd_append(
        &cmd,
        "-I/Users/nicorusso/Development/raylib/raylib-6.0_macos/include/",
        "-I/Users/nicorusso/Development/lua/lua-5.5.1/src");

    cmd_append(&cmd,
               "-L/Users/nicorusso/Development/raylib/raylib-6.0_macos/lib/",
               "-L/Users/nicorusso/Development/lua/lua-5.5.1/src");

    cmd_append(
        &cmd,
        "-Wl,-rpath,/Users/nicorusso/Development/raylib/raylib-6.0_macos/lib/");

    cmd_append(&cmd, "-o", "./main", "main.c");

    cmd_append(&cmd, "-lraylib", "-llua");

    // If you ever split this into several source files, add one
    // cc_db_add(&compdb, cmd, "...") per file (right before that file's
    // cmd_run) and move this cc_db_write() call to after the last one.
    cc_db_add(&compdb, cmd, "main.c");

    if (!cc_db_write(&compdb, "compile_commands.json"))
        return 1;

    if (!cmd_run(&cmd))
        return 1;

    if (run) {
        cmd_append(&cmd, "./main");
        da_append_many(&cmd, argv, argc);
        if (!cmd_run(&cmd))
            return 1;
    }

    return 0;
}

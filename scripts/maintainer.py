#! /usr/bin/env python3
# Copyright (c) 2021-2022 SanCloud Ltd
# SPDX-License-Identifier: MIT

import argparse
import os
import re
import shutil
import subprocess


def run(cmd, **kwargs):
    return subprocess.run(cmd, shell=True, check=True, **kwargs)


def capture(cmd, **kwargs):
    return run(cmd, capture_output=True, **kwargs).stdout.decode("utf-8")


def do_build(args):
    run("cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build .")
    run("cmake --build build")
    if args.docs:
        run("cmake --build build -t docs")
    if args.lint:
        run(
            "clang-tidy "
            "--checks=-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling "
            "-p build/compile_commands.json "
            "src/femtolog.c src/femtolog-example.c"
        )
        run(
            "cppcheck "
            "--enable=all "
            "--suppress=missingIncludeSystem "
            "--suppress=unusedFunction "
            "--inline-suppr "
            "--project=build/compile_commands.json"
        )


def do_clean(args):
    if os.path.exists("build"):
        shutil.rmtree("build")


def do_release(args):
    do_clean(args)

    args.release = True
    do_set_version(args)
    release_commit = capture("git rev-parse HEAD").strip()

    args.docs = True
    args.lint = False
    do_build(args)

    if os.path.exists("release"):
        shutil.rmtree("release")
    os.makedirs("release")
    run(
        f"tar czf release/femtolog-html-docs-{args.version}.tar.gz "
        f"--transform 's,^build/html,femtolog-html-docs-{args.version},' build/html"
    )
    run(
        f"git archive -o release/femtolog-{args.version}.tar.gz "
        f"--prefix=femtolog-{args.version}/ {release_commit}"
    )

    rel_notes = (
        capture(f"markdown-extract -n ^{args.version} ChangeLog.md").strip() + "\n"
    )
    with open(f"release/femtolog-release-notes-{args.version}.md", "w") as f_notes:
        f_notes.write(f"# Release Notes: femtolog v{args.version}\n\n")
        f_notes.write(rel_notes)

    file_list = " ".join(
        [
            f"femtolog-release-notes-{args.version}.md",
            f"femtolog-{args.version}.tar.gz",
            f"femtolog-html-docs-{args.version}.tar.gz",
        ]
    )
    with open("release/SHA256SUMS", "w") as f:
        text = capture(f"sha256sum {file_list}", cwd="release")
        f.write(text)
    with open("release/B3SUMS", "w") as f:
        text = capture(f"b3sum {file_list}", cwd="release")
        f.write(text)
    file_list += " SHA256SUMS B3SUMS"
    if args.sign:
        run("gpg --detach-sign -a release/SHA256SUMS")
        run("gpg --detach-sign -a release/B3SUMS")
        file_list += " SHA256SUMS.asc B3SUMS.asc"

    run(f"git tag -m 'femtolog v{args.version}' v{args.version} HEAD")
    if not args.no_gitlab:
        run("git push origin")
        run(f"git push origin {release_commit}:refs/heads/release")
        run(f"git push origin v{args.version}")
        run(
            f"glab release create v{args.version} -n 'femtolog v{args.version}' "
            f"-F- {file_list}",
            cwd="release",
            input=rel_notes,
            encoding="utf-8",
        )
    if not args.no_github:
        run("git push gh")
        run(f"git push gh {release_commit}:refs/heads/release")
        run(f"git push gh v{args.version}")
        run(
            f"gh release create v{args.version} -t 'femtolog v{args.version}' "
            f"-F- {file_list}",
            cwd="release",
            input=rel_notes,
            encoding="utf-8",
        )


def do_release_signatures(args):
    file_list = "SHA256SUMS.asc B3SUMS.asc"
    if not args.no_gitlab:
        run(f"glab release upload v{args.version} {file_list}", cwd="release")
    if not args.no_github:
        run(f"gh release upload v{args.version} {file_list}", cwd="release")


def do_set_version(args):
    with open("CMakeLists.txt", "r+") as f:
        text = re.sub(
            r"(project\(femtolog VERSION).*\n", rf"\1 {args.version})\n", f.read()
        )
        f.seek(0)
        f.write(text)
        f.truncate()
    msg = "Release" if args.release else "Bump version to"
    run(f'git commit -asm "{msg} {args.version}"')


def do_no_command(args):
    print("Missing command! Try `./scripts/maintainer.py --help`")


def parse_args():
    parser = argparse.ArgumentParser()
    parser.set_defaults(cmd_fn=do_no_command)
    subparsers = parser.add_subparsers(
        dest="cmd", title="Maintainer commands", metavar="command"
    )

    build_cmd = subparsers.add_parser(name="build", help="Build the project")
    build_cmd.set_defaults(cmd_fn=do_build)
    build_cmd.add_argument(
        "-d", "--docs", action="store_true", help="Build documentation"
    )
    build_cmd.add_argument(
        "-L",
        "--lint",
        action="store_true",
        help="Check code with clang-tidy & cppcheck",
    )

    clean_cmd = subparsers.add_parser(
        name="clean", help="Remove build output from the source tree"
    )
    clean_cmd.set_defaults(cmd_fn=do_clean)

    release_cmd = subparsers.add_parser(
        name="release", help="Release a new version of this project"
    )
    release_cmd.set_defaults(cmd_fn=do_release)
    release_cmd.add_argument("version", help="Version string for the new release")
    release_cmd.add_argument(
        "-s", "--sign", action="store_true", help="Sign release with gpg"
    )
    release_cmd.add_argument(
        "--no-gitlab",
        action="store_true",
        help="Disable push to SanCloud gitlab instance",
    )
    release_cmd.add_argument(
        "--no-github",
        action="store_true",
        help="Disable push to GitHub",
    )

    release_signatures_cmd = subparsers.add_parser(
        name="release-signatures",
        help="Push release signatures to GitHub and/or GitLab",
    )
    release_signatures_cmd.set_defaults(cmd_fn=do_release_signatures)
    release_signatures_cmd.add_argument(
        "version", help="Release to push signatures for (must already be released)"
    )
    release_signatures_cmd.add_argument(
        "--no-gitlab",
        action="store_true",
        help="Disable pushing signatures to SanCloud gitlab instance",
    )
    release_signatures_cmd.add_argument(
        "--no-github",
        action="store_true",
        help="Disable pushing signatures to public github repositories",
    )

    set_version_cmd = subparsers.add_parser(
        name="set-version", help="Set version string & commit"
    )
    set_version_cmd.set_defaults(cmd_fn=do_set_version)
    set_version_cmd.add_argument("version", help="New version string")
    set_version_cmd.add_argument(
        "-r",
        "--release",
        action="store_true",
        help="This version bump is for a release",
    )

    return parser.parse_args()


def main():
    args = parse_args()
    args.cmd_fn(args)


if __name__ == "__main__":
    main()

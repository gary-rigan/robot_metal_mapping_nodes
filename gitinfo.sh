#!/bin/sh
echo "============ git info v0.0.2============="
echo "Commit num:"
git rev-list --count HEAD

git fetch
git status
git tag
git branch -a
git remote -v


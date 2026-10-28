#!/bin/sh

echo "============ git info v0.0.2============="
echo "Commit num:"
git rev-list --count HEAD

time=`date "+%Y-%m-%d_%H:%M:%S"`

git status
git add .
git commit -m "daily ${time}"

for branch in `git branch --format='%(refname:short)'`; do
    git push --set-upstream robsys $branch
done

echo "Finished Push!"

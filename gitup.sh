#!/bin/sh

time=`date "+%Y-%m-%d_%H:%M:%S"`

git status
git add .
git commit -m "daily ${time}"

for branch in `git branch --format='%(refname:short)'`; do
    git push --set-upstream robsys $branch
done

echo "Finished Push!"

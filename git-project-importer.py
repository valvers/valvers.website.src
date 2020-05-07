#!/usr/bin/env python3

# We will use the
import os
from subprocess import run
import yaml
import shutil

scriptdir="."

with open("git-project-importer.yaml", "r") as rf:
    configuration = yaml.load(rf, Loader=yaml.SafeLoader)

if "git-projects" not in configuration:
    raise RuntimeError("git-projects is required in the top of the configuration")

if not os.path.isdir(".gitprojects"):
    os.mkdir(".gitprojects")

for project in configuration['git-projects']:
    if os.path.isdir(f".gitprojects/{project['name']}"):
        run(["git", "pull"], cwd=f".gitprojects/{project['name']}", check=True)
    else:
        run(["git", "clone", project['url'], project['name']], cwd=".gitprojects", check=True)

    if os.path.isdir(f".gitprojects/{project['name']}/images"):
        # If there is an images directory we will merge that into the site's images directory. Quite easy...
        if not os.path.isdir(f"docs/img/{project['name']}"):
            os.mkdir(f"docs/img/{project['name']}")

        shutil.copytree(f".gitprojects/{project['name']}/images", f"docs/img/{project['name']}",
                        dirs_exist_ok=True)

    if not os.path.isdir(f"docs/{project['root']}"):
        os.mkdir(f"docs/{project['root']}")

    # Always check for a root document
    if os.path.isfile(f".gitprojects/{project['name']}/readme.md"):
        with open(f".gitprojects/{project['name']}/readme.md", "rb") as rf:
            document = rf.read().decode('utf-8')

        # Get rid of any formatting differences, we just need \n for newlines
        document = document.replace("\r", "")
        document = document.replace("/images/", f"/img/{project['name']}/")

        with open(f"docs/{project['root']}/{project['name']}.md", "wb") as wf:
            wf.write(document.encode('utf-8'))

    root_list = os.listdir(f".gitprojects/{project['name']}")
    for f in root_list:
        if not os.path.isdir(f".gitprojects/{project['name']}/{f}"):
            continue
        if not os.path.isfile(f".gitprojects/{project['name']}/{f}/readme.md"):
            continue

        print(f"Including .gitprojects/{project['name']}/{f}/readme.md")
        with open(f".gitprojects/{project['name']}/{f}/readme.md", "rb") as rf:
            document = rf.read().decode('utf-8')

        # Get rid of any formatting differences, we just need \n for newlines
        document = document.replace("\r", "")
        document = document.replace("/images/", f"/img/{project['name']}/")

        with open(f"docs/{project['root']}/{project['name']}-{f}.md", "wb") as wf:
            wf.write(document.encode('utf-8'))

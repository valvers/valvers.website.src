#!/bin/sh

# (c)2020 Brian Sidebotham <brian.sidebotham@gmail.com>
# License: MIT


scriptdir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)

output_dir=""
api_token=""

if [ $# -lt 2 ]; then
    echo "usage: ${0} output-directory github-personal-api-token" >&2
    exit 1
fi

output_dir="${1}"
api_token="${2}"

# Firstly, make sure any projects managed outside of the website are kept up-to-date
${scriptdir}/git-project-importer.py

if [ $? -ne 0 ]; then
    echo "Could not import the git projects" >&2
    exit 1
fi

# Write the configuration to the yaml file
cp "${scriptdir}/mkdocs.template.yml" "${scriptdir}/mkdocs.yml"
sed -i "s/GIT_API_TOKEN/${api_token}/" "${scriptdir}/mkdocs.yml"

which mkdocs > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "mkdocs is required to build - see readme.md for instructions on how to install" >&2
    exit 1
fi

# Copying material template - this will need adjusting using the diff as a reference to put the
# valvers specific bits back in...
cd "${scriptdir}"
python3 - << EOF
import material
import os
from shutil import copyfile
copyfile(os.path.join(os.path.dirname(material.__file__), 'base.html'), "${scriptdir}/valvers-theme/base.html")
EOF

# Insert the required google-site-verification
sed -i "s/\s<meta charset=\"utf-8\">/<meta charset=\"utf-8\">\n    <meta name=\"google-site-verification\" content=\"gdf4XzjnbaArINbai3d8P0x_W3ugnDeIUyhMuq-wHPs\" \/>/" "${scriptdir}/valvers-theme/base.html"

cd "${scriptdir}" && mkdocs build
if [ $? -ne 0 ]; then
    echo "Failed to build website" >&2
    exit 1
fi

echo "Cleaning output directory"
# Be CAREFUL!
if [ "${output_dir}X" = "X" ]; then
    echo "Invalid output directory!" >&2
    exit 1
fi

rm -rf "${output_dir}"/*

# Make sure the custom domain CNAME is in place for GitHub pages...
echo -n "www.valvers.com" > ${output_dir}/CNAME

echo "Installing site to ${output_dir}"
mv "${scriptdir}"/site/* "${output_dir}"/

# Make sure we don't publish any source code to the website - don't bother
# hosting that. All source code should be referenced through github
rm -rf "${output_dir}"/src

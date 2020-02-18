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

# Write the configuration to the yaml file
cp "${scriptdir}/mkdocs.template.yml" "${scriptdir}/mkdocs.yml"
sed -i "s/GIT_API_TOKEN/${api_token}/" "${scriptdir}/mkdocs.yml"

which mkdocs > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "mkdocs is required to build - see readme.md for instructions on how to install" >&2
    exit 1
fi

# Clearing out the old site so we don't get any contamination
# List everything (except dot files) and discard source files
# website_files=$(cd "${scriptdir}" && ls -1 | grep -Ev '(^docs$|readme\.md|build\.sh|license|mkdocs\.yml|mkdocsvalvers-theme|valvers-assets)')
# while read -r f; do
#     echo "Removing" "${scriptdir}"/"${f}"
#     cd "${scriptdir}" && rm -rf "${f}"
# done << EOF
# ${website_files}
# EOF

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

echo "Installing site to ${output_dir}"
mv "${scriptdir}"/site/* "${output_dir}"/


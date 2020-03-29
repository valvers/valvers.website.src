#!/opt/wsl/bin/python3

# (c)2019 Brian Sidebotham <brian.sidebotham@gmail.com>
# Author: Brian Sidebotham <brian.sidebotham@gmail.com>

# THIS IS AN ARCHIVED SCRIPT AND ESSENTIALLY OF NO USE...

import pymysql
import pymysql.cursors
import re
import sys
import datetime
import os

src="md"

slugs = ["v6manta", "denford-orac", "v8manta"]
for slug in slugs:

    db = pymysql.connect("BLAH", "BLAH", "BLAH", "BLAH",
                         cursorclass=pymysql.cursors.DictCursor)

    cursor = db.cursor()
    cursor.execute("SELECT * FROM wp_terms")

    data = cursor.fetchone()

    term_id = None
    while data:
        if data['slug'] == slug:
            term_id = data['term_id']
            break

        data = cursor.fetchone()

    if term_id is None:
        print("Error: Cannot get the slug's term ID")
        sys.exit(1)

    cursor.execute(f"SELECT * FROM wp_term_relationships WHERE term_taxonomy_id={term_id}")
    data = cursor.fetchone()
    required_posts = []
    while data:
        required_posts.append(data)
        data = cursor.fetchone()

    posts = []
    for pid in required_posts:
        cursor.execute(f"SELECT * FROM wp_posts WHERE ID={pid['object_id']}")
        posts.append(cursor.fetchone())

    print(f"Found {len(posts)} {slug} posts")

    replacements = [
        ['<br><br>', '\n'],
        ['&#039;', '\''],
        ['<td>', ''],
        ['<tr>', ''],
        ['<table>', ''],
        ['</td>', ''],
        ['</tr>', ''],
        ['</table>', ''],
        ['<h2 id=\'\'>', ''],
        ['</h2>', ''],
        ['</div>', ''],
        ['<div>', ''],
        ['<div style=\'width:100%\'>', ''],
        ['<table style=\'width:100%\'>', ''],
        ['<td style=\'vertical-align:top;text-align:left;margin-left:10px;margin-right:10px;\'>', ''],
        ['http', 'https'],
        ['&nbsp;', ' ']
    ]


    regex_replacements = [
        # <img class="alignnone size-full wp-image-2641" title="DSCF6021" src="https://www.valvers.com/wp-content/uploads/2012/11/DSCF6021.jpg" alt="" width="1024" height="768" />
        [re.compile(r"<img.*src=['\"]([-a-z:/.0-9A-Z]+)['\"].*>"), r"![](\g<1>)"],

        # <h3 id='Worst bearing'>Worst bearing</h3>
        [re.compile(r"<h3[.]?>(.*)</h3>"), r"### \g<1>"],
        [re.compile(r"<h2[.]?>(.*)</h2>"), r"## \g<1>"],
        [re.compile(r"<h1[.]?>(.*)</h1>"), r"# \g<1>"],

        [re.compile(r"http[s]?://www.youtube.com/watch\?v=(.*)"), r'<iframe width="560" height="315" src="https://www.youtube.com/embed/\g<1>" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>'],
        [re.compile(r"http[s]?://www.valvers.com/images/manta(.*)"), r"/images/v6manta/manta\g<1>"]
    ]

    post_titles = []

    for post in posts:
        if post is None:
            print(f"POST IS NONE!")
            continue

        if 'post_date' not in post:
            print(f"NO POST_DATE: {post}")
            continue

        if 'post_content' not in post:
            print(f"NO POST_CONTENT: {post}")
            continue

        datestring = post['post_date'].date().isoformat()

        posttitle = post['post_title'].replace(' ', '-').lower()
        posturl = datestring.replace('-','/') + f"/{posttitle}"
        postdir = f"{src}/{posturl}"

        if not os.path.isdir(postdir):
            os.makedirs(postdir)

        for replacement in replacements:
            post['post_content'] = post['post_content'].replace(replacement[0], replacement[1])

        # Use the regular expression replacements to drive in-place alteration of the various elements
        for reg in regex_replacements:
            post['post_content'] = reg[0].sub(reg[1], post['post_content'])

        post_titles.append([post['post_title'], posturl])

        with open(f"{postdir}/index.md", "w") as f:
            f.write(post['post_content'])

    catdir = f"{src}/category/{slug}"
    if not os.path.isdir(catdir):
        os.makedirs(catdir)

    with open(f"{catdir}/index.md", "wt") as f:
        f.write("# V6 Opel Manta\n\n")

        for p in post_titles:
            # Make the post url absolute
            f.write(f"- [{p[0]}](/{p[1]})\n")

    db.close()

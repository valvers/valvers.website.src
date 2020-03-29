# Valvers Website

The mkdocs source code for the valvers.com website.

## License

Everything here is licensed under the [MIT](https://opensource.org/licenses/MIT) license.

I was going to go for the CC0 license - but it's too much of a faf and the MIT license
was as close as I could get to public domain.

## Building

Get the build dependencies and then run `mkdocs build OUTPUT_DIR [API_TOKEN]`

where the `OUTPUT_DIR` is the destination directory for the website and `API_TOKEN`
is an optional personal api access token for the git committers plugin.

### Fedora

```
python -m pip install mkdocs mkdocs-material mkdocs-git-committers-plugin --user
```

This gets you the mkdocs binary installed locally for your user:

```
$ which mkdocs
~/.local/bin/mkdocs
```

## Developing

Local development is done by adding/modifying the contents of the `docs` directory.

The site can be built with real-time change monitoring for developing changes by using

```
$ mkdocs serve
INFO    -  Building documentation...
INFO    -  Cleaning site directory
[I 200218 10:40:10 server:296] Serving on http://127.0.0.1:8000
[I 200218 10:40:10 handlers:62] Start watching changes
```

If you receive a build error like the following, it is because you have
uncommitted pages in this repository. `mkdocs` cannot use the git committers
plugin if there are no commits for a page.

You'll have to commit the page to the source repository before building.

```
ERROR   -  Error building page 'open-software/posix-shell-scripting.md': list index out of range
Traceback (most recent call last):
  File "/home/brian/.local/bin/mkdocs", line 10, in <module>
    sys.exit(cli())
  File "/home/brian/.local/lib/python3.7/site-packages/click/core.py", line 764, in __call__
    return self.main(*args, **kwargs)
  File "/home/brian/.local/lib/python3.7/site-packages/click/core.py", line 717, in main
    rv = self.invoke(ctx)
  File "/home/brian/.local/lib/python3.7/site-packages/click/core.py", line 1137, in invoke
    return _process_result(sub_ctx.command.invoke(sub_ctx))
  File "/home/brian/.local/lib/python3.7/site-packages/click/core.py", line 956, in invoke
    return ctx.invoke(self.callback, **ctx.params)
  File "/home/brian/.local/lib/python3.7/site-packages/click/core.py", line 555, in invoke
    return callback(*args, **kwargs)
  File "/home/brian/.local/lib/python3.7/site-packages/mkdocs/__main__.py", line 163, in build_command
    ), dirty=not clean)
  File "/home/brian/.local/lib/python3.7/site-packages/mkdocs/commands/build.py", line 295, in build
    _build_page(file.page, config, files, nav, env, dirty)
  File "/home/brian/.local/lib/python3.7/site-packages/mkdocs/commands/build.py", line 212, in _build_page
    'page_context', context, page=page, config=config, nav=nav
  File "/home/brian/.local/lib/python3.7/site-packages/mkdocs/plugins.py", line 94, in run_event
    result = method(item, **kwargs)
  File "/home/brian/.local/lib/python3.7/site-packages/mkdocs_git_committers_plugin/plugin.py", line 76, in on_page_context
    context['last_commit'] = self.get_last_commit(git_path)
  File "/home/brian/.local/lib/python3.7/site-packages/mkdocs_git_committers_plugin/plugin.py", line 46, in get_last_commit
    last = commits[0]
  File "/home/brian/.local/lib/python3.7/site-packages/github/PaginatedList.py", line 51, in __getitem__
    return self.__elements[index]
IndexError: list index out of range
Failed to build website
```

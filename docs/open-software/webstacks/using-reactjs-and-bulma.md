# Using ReactJS and Bulma

An extremely powerful combination for web development is a combination of ReactJS
(for the HTML view) and Bulma for the css styling of the application. It's a
great web application starter.

>**NOTE:** There is a [github repository](https://github.com/BrianSidebotham/reactjs-bulma-tutorial) with the code in for this tutorial.

## Getting Started

In order to get started, we need a new ReactJS application. Go ahead,
and as per the [online ReactJS tutorial](https://reactjs.org/docs/hello-world.html), get
a new project up and running with:

```
npx create-react-app reactjs-bulma-tutorial
```

Make sure the project has built ok by testing it out:

```
cd reactjs-bulma-tutorial
npm start
```

Your browser should point you to a ReactJS application that looks a bit like this:

![](/img/webstacks/reactjs/getting-started-reactjs.png)

Great - we've got our ReactJS demo started. Next, we'll add Bulma in.

### Installing Bulma

In order to get started, we need a ReactJS wrapper for Bulma. There is a great
one, [react-bulma-components](https://github.com/couds/react-bulma-components)
which wraps Bulma for use in ReactJS applications.

From within the root of the new project type:

```
npm install --save react-bulma-components
```

This installs the Bulma ReactJS components ready for us to import into our
project.

## Adding a Bulma Component

Bulma has a lot of components, all wrapped up and waiting for you to use in
your application. But let's start out right from the beginning. One major advantage
of modern CSS is SCSS or SASS which provide us with compilable CSS - enabling us to
theme and adjust settings using variables. It's really handy to have that stuff
and as with everything, it's best to bake it in a the start.

### Adding SASS Support

From inside the project root, install the `node-sass` module:

```
npm install --save node-sass
```

In the project root, create a new file called `jsconfig.json` with the following
content:

```json
{
    "compilerOptions": {
        "baseUrl": "./src"
    }
}
```

In the `/src/` directory create a new file called `_variables.sass`. This is
what the Bulma SASS will use to override CSS variables. Even if it's unused, it
should be included in the project to prevent the Bulma SASS from failing to
compile.

Rename `/src/App.css` to `/src/App.scss` so it will get compiled rather than
just be included.

At the top of the `/src/App.scss` file, add the following line to import the
Bulma (Compilable) css:

```javascript
@import "~react-bulma-components/src/index.sass";
```

In the `/src/App.js` source file, change the `App.css` import to be `App.scss` now.


### Add a Bulma button

We can noe import a `button` for example to use:

At the top of `/src/App.js`, underneath the `import React...` line, you can add
the following to import the Button component:

```javascript
import Button from 'react-bulma-components/lib/components/button';
```

Now we can go ahead and add the imported button component to the page. Let's add
the button component under the main paragraph text in the example. Immediately
after the paragraph lets add the button as simply as we can:

```html
<Button color='primary'>Press Me!</Button>
```

Now when you run `npm start` in the project's root you should see something with
a Bulma button in:

![](/img/webstacks/reactjs/getting-started-reactjs-sass.png)

The SASS enables us to change CSS colours easily. In the `src/_variables.sass` file
add the following line to change the primary colour used by Bulma SASS:

```
$primary: #ffaa55;
```

Now, you'll see (perhaps after a page reload) that the c;olour of the button has
turned orange. All components basing themselves off of the primary colour will be
affected throughout the CSS stack.

![](/img/webstacks/reactjs/getting-started-reactjs-sass-orange.png)


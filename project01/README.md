## About

Setting up an old ray-tracer project to work that I built in college. I've wanted to put this thing in github with instructions to set it up.

10 years later, here it is.

## Building

``` bash
$ cd project01
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Running

``` bash
$ cd project01
$ ./build/bin/ray-tracing prog01.scn 
$ eog image.ppm
```

## Output

<div class="card">
  <div class="card-image">
    <img src="image.png">
  </div>
  <div class="card-action">
    <a href="prog01.scn">Open scene file</a>
  </div>
</div>

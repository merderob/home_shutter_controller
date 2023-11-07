// Copyright © 2023 Robert Takacs
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

const char index_page [] PROGMEM = R"=====(
<!DOCTYPE html><html>
<head>
<title>shutter control</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">

<script>
	function setAction(form) {
		console.log("oooo");
		console.log(form.shutter_scale.value);
		form.action = "/get?kok=" + form.living_room_door.checked + "," + form.living_room_window.checked;
		return true; 
	}
</script>

<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
.button {
	border: none; color: white; 
	padding: 20px 22px;
	text-decoration: none; font-size: 24px; margin: 2px; 
	cursor: pointer; 
	width: 70px;
	border-radius: 8px;}
.outer {width:100%; text-align: center; }
.inner {display: inline-block; }
.button.r1{background-color: #66e3e3; }
.button.r2{background-color: white; color: #b9b9b9; border: 2px solid #66e3e3b2;}
.button.r3{background-color: #66e3e3; transform: rotate(180deg);} 
.button.lg{background-color: #b9b9b9; width: 200px;}
input.largerCheckbox {
width: 40px;
height: 40px;
}

.slider {
  height: 105px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 100px;
  border: 0;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 100px;
  background: #472701;
  border: 0;
  cursor: pointer;
}

</style></head>


<body style="background-color:#fcfaf2;"><h2>shutter control</h2>
<p>manual control</p>
<div class="outer">
	<div class ="inner"><a href="/get?command=3,up"><button class="button r1"> &#916; </button></a></div>
	<div class ="inner"><a href="/get?command=2,up"><button class="button r1"> &#916;</button></a></div>
	<div class ="inner"><a href="/get?command=1,up"><button class="button r1">&#916;</button></a></div>
	<div class ="inner"><a href="/get?command=0,up"><button class="button r1">&#916;</button></a></div>
</div>
<div class="outer">
	<div class ="inner"><a href="/get?command=3,stop"><button class="button r2"> &#9634; </button></a></div>
	<div class ="inner"><a href="/get?command=2,stop"><button class="button r2"> &#9634; </button></a></div>
	<div class ="inner"><a href="/get?command=1,stop"><button class="button r2"> &#9634; </button></a></div>
	<div class ="inner"><a href="/get?command=0,stop"><button class="button r2"> &#9634; </button></a></div>
</div>
<div class="outer">
	<div class ="inner"><a href="/get?command=3,down"><button class="button r3"> &#916; </button></a></div>
	<div class ="inner"><a href="/get?command=2,down"><button class="button r3">  &#916; </button></a></div>
	<div class ="inner"><a href="/get?command=1,down"><button class="button r3">  &#916; </button></a></div>
	<div class ="inner"><a href="/get?command=0,down"><button class="button r3">  &#916; </button></a></div>
</div>
<br/>
<p>control multiple shutters</p>
<br/>
<form name="my_form" onsubmit="return setAction(this)">
	<div class="outer" style="transform: translate(0px, 30px) rotate(90deg);">
		<input class="slider" type="range" name="shutter_scale" min="1" max="100" value="50">
	</div>
	<div class="outer" style="transform: translate(0px, 60px);">
		<input type="checkbox" class="largerCheckbox" name="living_room_door">
		<input type="checkbox" class="largerCheckbox" name="living_room_window">
		<input type="checkbox" class="largerCheckbox" name="bedroom_door">
		<input type="checkbox" class="largerCheckbox" name="bedroom_window">
		</div>
	<input class="button lg" type="submit" style="transform: translate(0px, 60px)"  value="set slider">
</form>
</body>
</html>

)=====";
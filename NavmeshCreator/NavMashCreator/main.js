var POINT_BAR_WITDTH = 150;

displayImageOnCanvas(true);
test_function()
console.log(render_info.test)



function draw_line()
{
  var c = document.getElementById("myCanvas");
  var ctx = c.getContext("2d");
  ctx.moveTo(0, 0);
  ctx.lineTo(200, 100);
  ctx.stroke();
}

function draw_circle(canvas, y)
{
  var canvas_context = canvas.getContext("2d");
  var radius = 30;

  canvas_context.beginPath();
  canvas_context.arc(canvas.width - (POINT_BAR_WITDTH / 2), y, radius, 0, 2 * Math.PI, false);
  canvas_context.fillStyle = 'red';
  canvas_context.fill();
  canvas_context.lineWidth = 2;
  canvas_context.strokeStyle = '#003300';
  canvas_context.stroke();
  /*
  */
}

function displayImageOnCanvas(debug)
{
  var canvas = document.getElementById("myCanvas");
  var canvas_context = canvas.getContext("2d");

  file_input = document.getElementById("image-input");
  if(!debug && (!file_input || !file_input.files || !file_input.files[0]))
  {
    console.log("Error loading file");
    return;
  }


  var background = new Image();
  if (debug)
    background.src = "../MapImages/1v1Map.PNG"
  else
    background.src = "../MapImages/" + file_input.files[0].name;

  // Make sure the image is loaded first otherwise nothing will draw.
  background.onload = function()
  {
    canvas.width = background.width + POINT_BAR_WITDTH;
    canvas.height = background.height;
    canvas_context.lineWidth = 10;
    canvas_context.strokeRect(0, 0, canvas.width, canvas.height);
    
    canvas_context.drawImage(background,0,0);  
    draw_circle(canvas, 100);
    draw_circle(canvas, 300);
    draw_circle(canvas, 500);
    draw_circle(canvas, 700);
  }
}

function processFile()
{
    element = document.getElementById("file-input");
    imgelement = document.getElementById("testimg");
    imgelement.src = "../MapImages/" + element.files[0].name;

    console.log(element.files[0]);
}

function export2txt() {
  const originalData = {
    members: [{
        name: "cliff",
        age: "34"
      },
      {
        name: "ted",
        age: "42"
      },
      {
        name: "bob",
        age: "12"
      }
    ]
  };

  const a = document.createElement("a");
  a.href = URL.createObjectURL(new Blob([JSON.stringify(originalData, null, 2)], {
    type: "text/plain"
  }));
  a.setAttribute("download", "data.txt");
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
}

function loadImage()
{
  alert("Hallo")
}

function loadFile() {
  var input, file, fr;

  if (typeof window.FileReader !== 'function') 
  {
    alert("The file API isn't supported on this browser yet.");
    return;
  }

  input = document.getElementById('json-input');
  if (!input || !input.files ||!input.files[0]) 
  {
    alert("Problem getting json");
    return;
  }

  file = input.files[0];
  fr = new FileReader();
  fr.onload = load_json;
  fr.readAsText(file);

  function load_json(e) 
  {
    let lines = e.target.result;
    var json = JSON.parse(lines); 

    console.log(json)
  }
}
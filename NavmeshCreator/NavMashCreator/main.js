var debug = true;

if(debug) 
{
  load_image(true);
//  load_file(true);
}

function test_func()
{
  clear_canvas()
}

function load_image(debug)
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

function load_file() {
  var input, file, file_reader;

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
  file_reader = new FileReader();
  file_reader.onload = load_json;
  file_reader.readAsText(file);

  function load_json(e) 
  {
    let lines = e.target.result;
    var json = JSON.parse(lines); 

    console.log(json)
  }
}
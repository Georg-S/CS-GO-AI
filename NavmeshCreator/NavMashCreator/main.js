displayImageOnCanvas(true);


function draw_line()
{
  var c = document.getElementById("myCanvas");
  var ctx = c.getContext("2d");
  ctx.moveTo(0, 0);
  ctx.lineTo(200, 100);
  ctx.stroke();
}

function displayImageOnCanvas(debug)
{
  var canvas = document.getElementById("myCanvas");
  var canvas_context = canvas.getContext("2d");

  file_input = document.getElementById("file-input");
  var background = new Image();
  if (debug)
    background.src = "../MapImages/1v1Map.PNG"
  else
    background.src = "../MapImages/" + file_input.files[0].name;

  // Make sure the image is loaded first otherwise nothing will draw.
  background.onload = function()
  {
    canvas.width = background.width;
    canvas.height = background.height;
    
    canvas_context.drawImage(background,0,0);  
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
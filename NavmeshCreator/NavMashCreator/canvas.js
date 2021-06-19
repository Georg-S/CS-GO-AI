var POINT_BAR_WITDTH = 150;


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
}

function clear_canvas()
{
    var canvas = document.getElementById("myCanvas");
    const context = canvas.getContext('2d');
    context.clearRect(0, 0, canvas.width, canvas.height);
}
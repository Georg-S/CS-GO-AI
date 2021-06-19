var canvas = new Canvas()
var navmesh_json = null


var debugging = true;
var debug_json = {"nodes":[{"corner":true,"id":0,"x":-699.9881591796875,"y":495.96875,"z":221.91024780273438},{"corner":true,"id":1,"x":-701.8301391601563,"y":-495.96875,"z":221.91024780273438},{"corner":true,"id":2,"x":958.2091674804688,"y":495.96875,"z":224.03125},{"corner":true,"id":3,"x":958.001220703125,"y":-495.96875,"z":224.03125},{"corner":false,"id":4,"x":824.4160766601563,"y":-115.69774627685547,"z":224.03125},{"corner":false,"id":5,"x":853.031982421875,"y":190.0935821533203,"z":224.03125},{"corner":false,"id":6,"x":614.6531982421875,"y":-64.1895523071289,"z":224.03125},{"corner":false,"id":7,"x":432.51898193359375,"y":-9.475232124328613,"z":226.20025634765625},{"corner":false,"id":8,"x":243.07858276367188,"y":4.7137064933776855,"z":226.20025634765625},{"corner":false,"id":9,"x":243.26971435546875,"y":-129.19692993164063,"z":226.20025634765625},{"corner":false,"id":10,"x":106.74373626708984,"y":-118.97126007080078,"z":226.20025634765625},{"corner":false,"id":11,"x":-9.509355545043945,"y":-123.79820251464844,"z":226.20025634765625},{"corner":false,"id":12,"x":0.716342031955719,"y":12.727855682373047,"z":226.20025634765625},{"corner":false,"id":13,"x":14.463363647460938,"y":135.8349609375,"z":226.20025634765625},{"corner":false,"id":14,"x":150.98936462402344,"y":125.60929870605469,"z":226.20025634765625},{"corner":false,"id":15,"x":257.79022216796875,"y":117.61000061035156,"z":226.20025634765625},{"corner":false,"id":16,"x":-159.70172119140625,"y":5.795236110687256,"z":226.20025634765625},{"corner":false,"id":17,"x":-342.41650390625,"y":19.480432510375977,"z":221.91024780273438},{"corner":false,"id":18,"x":-448.8756408691406,"y":1.9604183435440063,"z":221.91024780273438},{"corner":false,"id":19,"x":-460.0126037597656,"y":-146.73117065429688,"z":221.91024780273438},{"corner":false,"id":20,"x":-638.4385375976563,"y":-271.19158935546875,"z":221.91024780273438},{"corner":false,"id":21,"x":-638.6206665039063,"y":184.59576416015625,"z":221.91024780273438},{"corner":false,"id":22,"x":-361.45367431640625,"y":382.56427001953125,"z":278.9102478027344},{"corner":false,"id":23,"x":626.1458129882813,"y":-374.5694885253906,"z":281.03125}]}

canvas.canvas.addEventListener("mousemove", mouse_move);

function mouse_move(e)
{
  var cRect = canvas.canvas.getBoundingClientRect();        // Gets CSS pos, and width/height
  var canvasX = Math.round(e.clientX - cRect.left);  // Subtract the 'left' of the canvas 
  var canvasY = Math.round(e.clientY - cRect.top);   // from the X/Y positions to make  

  canvas.on_mouse_move(canvasX, canvasY);
}

function on_canvas_click()
{
  canvas.on_click();
}

if(debugging) 
{
  load_image(true);
  load_file(true);
}

function test_func()
{
  canvas.clear_canvas()
}

function load_image(debug)
{
  var canvas_buf = document.getElementById("myCanvas");
  var canvas_context = canvas_buf.getContext("2d");

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
    canvas.set_map(new Map(background))
    canvas.update_node_position_for_start(canvas.nodes)
    canvas.update_rendering();
  }
}

function load_file(debug) {
  if(debug)
  {
    navmesh_json = debug_json
    create_points(navmesh_json);
    return;  
  }

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
  create_points(navmesh_json);
  canvas.update_node_position_for_start(canvas.nodes)
  canvas.update_rendering();
}

function load_json(e) 
{
  let lines = e.target.result;
  navmesh_json = JSON.parse(lines);
}

function create_points(nodes_json)
{
  canvas.nodes = []
  for(node of nodes_json["nodes"])
    canvas.nodes.push(new Node(node.id, node.corner, node.x, node.y))

  canvas.update_node_position_for_start(canvas.nodes)
}

function getMousePos(canvas, evt) {
  var rect = canvas.getBoundingClientRect();
  return {
    x: evt.clientX - rect.left,
    y: evt.clientY - rect.top
  };
}
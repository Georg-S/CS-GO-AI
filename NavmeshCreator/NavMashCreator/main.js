var canvas = new Canvas()
var navmesh_json = null

var debugging = true;
var debug_json = {"nodes":[{"corner":true,"id":0,"x":-503.1479797363281,"y":495.96875,"z":221.91024780273438},{"corner":true,"id":1,"x":766.1019897460938,"y":-495.96875,"z":224.03125},{"corner":false,"id":2,"x":958.001220703125,"y":-495.96875,"z":224.03125},{"corner":false,"id":3,"x":958.2091674804688,"y":495.96875,"z":224.03125},{"corner":false,"id":4,"x":880.373779296875,"y":193.9611358642578,"z":224.03125},{"corner":false,"id":5,"x":872.446533203125,"y":-136.33201599121094,"z":224.03125},{"corner":false,"id":6,"x":666.1461791992188,"y":-7.625237464904785,"z":224.03125},{"corner":false,"id":7,"x":428.9729309082031,"y":2.160067081451416,"z":226.20025634765625},{"corner":false,"id":8,"x":266.1965026855469,"y":-1.9210280179977417,"z":226.20025634765625},{"corner":false,"id":9,"x":269.43804931640625,"y":-131.21153259277344,"z":226.20025634765625},{"corner":false,"id":10,"x":269.63360595703125,"y":136.3674774169922,"z":226.20025634765625},{"corner":false,"id":11,"x":132.83108520507813,"y":132.9375762939453,"z":226.20025634765625},{"corner":false,"id":12,"x":-0.22264492511749268,"y":129.60166931152344,"z":226.20025634765625},{"corner":false,"id":13,"x":-3.0361080169677734,"y":11.20258617401123,"z":226.20025634765625},{"corner":false,"id":14,"x":8.264240264892578,"y":-126.13407897949219,"z":226.20025634765625},{"corner":false,"id":15,"x":146.60435485839844,"y":-118.68836975097656,"z":226.20025634765625},{"corner":false,"id":16,"x":-253.52734375,"y":6.965755462646484,"z":227.91024780273438},{"corner":false,"id":17,"x":-453.65203857421875,"y":-8.26928997039795,"z":221.91024780273438},{"corner":false,"id":18,"x":-551.7155151367188,"y":-186.0554962158203,"z":221.91024780273438},{"corner":false,"id":19,"x":-701.8301391601563,"y":-495.96875,"z":221.91024780273438},{"corner":false,"id":20,"x":-623.5521850585938,"y":174.90281677246094,"z":221.91024780273438},{"corner":false,"id":21,"x":-699.9881591796875,"y":495.96875,"z":221.91024780273438},{"corner":false,"id":22,"x":-370.9281005859375,"y":136.001220703125,"z":221.91024780273438},{"corner":false,"id":23,"x":-362.3751525878906,"y":422.8355712890625,"z":278.9102478027344}]}

canvas.canvas.addEventListener("mousemove", mouse_move);

function mouse_move(e)
{
  var cRect = canvas.canvas.getBoundingClientRect();        // Gets CSS pos, and width/height
  var canvasX = Math.round(e.clientX - cRect.left);         // Subtract the 'left' of the canvas 
  var canvasY = Math.round(e.clientY - cRect.top);          // from the X/Y positions to make  

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

function place_allNodes()
{
  canvas.place_and_render_all_nodes();
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
}

function load_json(e) 
{
  let lines = e.target.result;
  navmesh_json = JSON.parse(lines);
  create_points(navmesh_json);
  canvas.update_node_position_for_start(canvas.nodes)
  canvas.update_rendering();
}

function create_points(nodes_json)
{
  canvas.nodes = []
  for(let node of nodes_json["nodes"])
    canvas.nodes.push(new Node(node.id, node.corner, node.x, node.y, node.z))

  canvas.update_node_position_for_start(canvas.nodes)
}

function save_created_json()
{
  navmesh_json.edges = canvas.edges;
  save_json(navmesh_json);
}

function save_json(json_input)
 {
  const a = document.createElement("a");
  a.href = URL.createObjectURL(new Blob([JSON.stringify(json_input, null, 2)], { type: "text/plain" }));
  a.setAttribute("download", "nav_mesh.json");
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
}
var POINT_BAR_WITDTH = 150;

class Node
{
    constructor(id, corner, game_x, game_y)
    {
        this.id = id
        this.corner = corner
        this.radius = 10;
        this.canvas_x = -100;
        this.canvas_y = -100;
        this.game_x = game_x;
        this.game_y = game_y;

        if(corner == null)
            this.corner = "false"
    }
}

class Map 
{
    constructor(map)
    {
        this.image = map;
        this.width = map.width;
        this.height = map.height;
    }
}

class Canvas
{
    constructor()
    {
        this.canvas = document.getElementById("myCanvas");
        this.map = null;
        this.nodes = [];
        this.only_draw_corner_nodes = true;
        this.mouse_x = 0;
        this.mouse_y = 0;
        this.selected_node = null;
        this.last_selected_node = null;
        this.canvas_distance_per_game_distance_x = null;
        this.canvas_distance_per_game_distance_y = null;
    }

    set_map(map)
    {
        this.map = map;
        this.canvas.width = this.map.width + POINT_BAR_WITDTH;
        this.canvas.height = this.map.height;
    }

    update_rendering()
    {
        this.clear_canvas();
        this.draw_map(this.map);
        this.draw_canvas_border();
        this.draw_nodes(this.nodes);
    }

    draw_map(map)
    {
        if(map == null || map.image == null)
            return;

        var context = this.canvas.getContext("2d");
        context.drawImage(map.image,0,0);  
    }

    draw_canvas_border()
    {
        var context = this.canvas.getContext("2d");

        context.lineWidth = 10;
        context.strokeRect(0, 0, this.canvas.width, this.canvas.height);
    }

    update_node_position_for_start(nodes)
    {

        if(nodes == null || this.map == null)
            return;
        
        var count = 0;
        const top_offset = 50;
        for(node of this.nodes) 
        {
            if(node["corner"] === true) 
            {
                node.canvas_x = this.canvas.width - (POINT_BAR_WITDTH / 2);
                node.canvas_y = 180 * count + top_offset;
                count++;
            }
        }
    }

    draw_nodes(nodes)
    {
        if(nodes == null || this.map == null)
            return;

        for(node of this.nodes) 
        {
            if(!this.only_draw_corner_nodes || node["corner"] === true) 
                this.draw_Node(node);
        }
    }

    draw_line()
    {
        context = this.canvas.getContext("2d");

        context.moveTo(0, 0);
        context.lineTo(200, 100);
        context.stroke();
    }

    draw_circle(y)
    {
        var radius = 30;
        var context = this.canvas.getContext("2d");

        context.beginPath();
        context.arc(this.canvas.width - (POINT_BAR_WITDTH / 2), y, radius, 0, 2 * Math.PI, false);
        context.fillStyle = 'red';
        context.fill();
        context.lineWidth = 2;
        context.strokeStyle = '#003300';
        context.stroke();
    }

    clear_canvas()
    {
        this.canvas.getContext("2d").clearRect(0, 0, this.canvas.width, this.canvas.height);
    }

    draw_Node(node) 
    {
        var context = this.canvas.getContext("2d");

        context.beginPath();
        context.arc(node.canvas_x, node.canvas_y, node.radius, 0, 2 * Math.PI, false);
        context.fillStyle = 'red';
        context.fill();
        context.lineWidth = 2;
        context.strokeStyle = '#003300';
        context.stroke();

        this.draw_node_information(node,"ID: " + node.id, 0);
        this.draw_node_information(node,"X: " + node.game_x,  1);
        this.draw_node_information(node,"Y: " + node.game_y,  2);
    }

    draw_node_information(node, text, y_offset)
    {
        var font_size = 15;
        var context = this.canvas.getContext("2d");
        var diameter = 2 * node.radius;

        context.font = font_size + "px Arial";
        context.fillStyle = 'black';
        context.fillText(text, node.canvas_x - diameter, node.canvas_y + diameter + y_offset * font_size);
    }

    width()
    {
        return this.width;
    }

    height()
    {
        return this.height;
    }

    on_mouse_move(x, y)
    {
        this.mouse_x = x;
        this.mouse_y = y;

        if(this.selected_node == null)
            return;
        
        this.selected_node.canvas_x = this.mouse_x;
        this.selected_node.canvas_y = this.mouse_y;
        this.update_rendering();
    }

    get_distance(x1, y1, x2, y2)
    {
        var x = x1 - x2;
        var y = y1 - y2;

        return Math.sqrt(x * x + y * y);
    }

    get_node_clicked()
    {
        for(node of this.nodes)
        {
            let distance = this.get_distance(node.canvas_x, node.canvas_y, this.mouse_x, this.mouse_y);
            if(distance <= node.radius)
                return node;
        }
        return null;
    }

    on_click()
    {
        if(this.selected_node == null)
        {
            this.selected_node = this.get_node_clicked();

            if(this.selected_node == null)
                return;

            var selected = document.getElementById("selected-node");
            selected.value = this.selected_node.id;
            return;
        }
        
        this.last_selected_node = this.selected_node;
        this.selected_node = null;
    }

    update_nodes_canvas_position_relative_to_node(reference_node)
    {
        for(node of this.nodes) 
        {
            if(reference_node.id == node.id)
                continue;

            var x = node.game_x - reference_node.game_x;
            var y = node.game_y - reference_node.game_y;

            node.canvas_x = reference_node.canvas_x + (x * this.canvas_distance_per_game_distance_x)
            node.canvas_y = reference_node.canvas_y - (y * this.canvas_distance_per_game_distance_y)
        }
    }

    draw_all_nodes()
    {
        var corner1 = null;
        var corner2 = null;

        for(node of this.nodes)
        {
            if(!node.corner)
                continue;
            
            if(corner1 == null)
                corner1 = node;
            else if(corner2 == null)
                corner2 = node;
        }
        
        var game_distance_x = Math.abs(corner1.game_x - corner2.game_x);
        var game_distance_y = Math.abs(corner1.game_y - corner2.game_y);
        var canvas_distance_x = Math.abs(corner1.canvas_x - corner2.canvas_x);
        var canvas_distance_y = Math.abs(corner1.canvas_y - corner2.canvas_y);

        this.canvas_distance_per_game_distance_x = canvas_distance_x / game_distance_x
        this.canvas_distance_per_game_distance_y = canvas_distance_y / game_distance_y
        this.only_draw_corner_nodes = false;
        this.update_nodes_canvas_position_relative_to_node(corner1);
        this.update_rendering();
    }
}

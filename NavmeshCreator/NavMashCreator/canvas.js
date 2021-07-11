const POINT_BAR_WITDTH = 150;


class ModeRadio
{
    static operation_radio_name = "operation_mode";
    static operation_radio_node_value = "node_placing";
    static operation_radio_edge_value = "edge_creation";

    static is_mode_node_moving()
    {
        return this.is_operation_mode_checked(this.operation_radio_node_value);
    }

    static is_mode_edge_creation()
    {
        return this.is_operation_mode_checked(this.operation_radio_edge_value);
    }

    static set_mode_to_node_moving()
    {
        this.set_operation_mode_checked(this.operation_radio_node_value);
    }

    static set_mode_to_edge_creation()
    {
        this.set_operation_mode_checked(this.operation_radio_edge_value);
    }

    static is_operation_mode_checked(mode)
    {   
        var radios = document.getElementsByName('operation_mode');

        for (var i = 0, length = radios.length; i < length; i++) 
        {
            if (radios[i].checked) 
                return mode === radios[i].value
        }

        return false;
    }

    static set_operation_mode_checked(name) 
    {
        var radios = document.getElementsByName('operation_mode');

        for (var i = 0, length = radios.length; i < length; i++) 
        {
            if (radios[i].value == name) 
            {
                radios[i].checked = true;
                return true;
            }
        }

        return false;
    }
}

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
        this.edges = [];
        this.only_draw_corner_nodes = true;
        this.mouse_x = 0;
        this.mouse_y = 0;
        this.selected_node = null;
        this.show_additional_node_information = true;
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
        this.draw_edges(this.edges);
    }

    draw_edges(edges)
    {
        if(edges == null || this.map == null)
            return;

        for(var edge of this.edges) 
            this.draw_edge(edge);
    }

    draw_edge(edge) 
    {

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
        for(var node of this.nodes) 
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

        for(let node of this.nodes) 
        {
            if(!this.only_draw_corner_nodes || node["corner"] === true) 
                this.draw_Node(node);
        }
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

        if(this.show_additional_node_information) 
        {
            this.draw_node_information(node,"ID: " + node.id, 0);
            this.draw_node_information(node,"X: " + node.game_x,  1);
            this.draw_node_information(node,"Y: " + node.game_y,  2);
        }
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

    clear_canvas()
    {
        this.canvas.getContext("2d").clearRect(0, 0, this.canvas.width, this.canvas.height);
    }

    draw_line()
    {
        context = this.canvas.getContext("2d");

        context.moveTo(0, 0);
        context.lineTo(200, 100);
        context.stroke();
    }

    on_mouse_move(x, y)
    {
        this.mouse_x = x;
        this.mouse_y = y;

        if(ModeRadio.is_mode_node_moving()) 
            this.on_mouse_move_node_placing(x, y);
    }

    on_mouse_move_node_placing(x, y)
    {
        if(this.selected_node == null)
            return;
        
        this.selected_node.canvas_x = this.mouse_x;
        this.selected_node.canvas_y = this.mouse_y;
        this.update_rendering();
    }

    get_distance_2d(x1, y1, x2, y2)
    {
        var x = x1 - x2;
        var y = y1 - y2;

        return Math.sqrt(x * x + y * y);
    }

    on_click()
    {
        if(ModeRadio.is_mode_node_moving())
            this.on_click_node_placing();
        else
            this.on_click_edge_creation();
    }

    on_click_node_placing()
    {
        if(this.selected_node == null)
        {
            this.selected_node = this.get_node_clicked();

            if(this.selected_node == null)
                return;

            this.set_text_of_selected_node_textbox(this.selected_node.id);
            return;
        }
        
        this.deselect_node();
    }

    get_node_clicked()
    {
        for(let node of this.nodes)
        {
            let distance = this.get_distance_2d(node.canvas_x, node.canvas_y, this.mouse_x, this.mouse_y);
            if(distance <= node.radius)
                return node;
        }
        return null;
    }

    set_text_of_selected_node_textbox(text)
    {
        var selected = document.getElementById("selected-node");
        selected.value = text;
    }

    deselect_node()
    {
        this.selected_node = null;
        this.set_text_of_selected_node_textbox("None")
    }

    on_click_edge_creation()
    {
        var selected = this.get_node_clicked();

       if (this.selected_node == null) 
       {
            if(selected == null)
                return;

            this.select_node(selected);
       }
       else if(selected != null) 
       {
            this.create_edge(this.selected_node, selected);
            this.deselect_node();
            this.update_rendering();
       }
       else 
       {
           this.deselect_node();
       }
    }

    select_node(node) 
    {
        this.selected_node = node;
        this.set_text_of_selected_node_textbox(node.id)
    }

    create_edge(node1, node2)
    {
        var json_edge = {"from" : node1.id, "to" : node2.id};

        this.edges.push(json_edge);
        console.log(this.edges);
    }

    place_and_render_all_nodes()
    {
        this.update_all_node_positions();
        this.update_rendering();
        ModeRadio.set_mode_to_edge_creation();
        this.deselect_node();
    }

    update_all_node_positions()
    {
        var [corner1, corner2] = this.get_corner_nodes();
        
        var game_distance_x = Math.abs(corner1.game_x - corner2.game_x);
        var game_distance_y = Math.abs(corner1.game_y - corner2.game_y);
        var canvas_distance_x = Math.abs(corner1.canvas_x - corner2.canvas_x);
        var canvas_distance_y = Math.abs(corner1.canvas_y - corner2.canvas_y);

        var canvas_distance_per_game_distance_x = canvas_distance_x / game_distance_x
        var canvas_distance_per_game_distance_y = canvas_distance_y / game_distance_y
        this.only_draw_corner_nodes = false;
        this.show_additional_node_information = false;
        this.update_nodes_canvas_position_relative_to_reference_node(corner1, canvas_distance_per_game_distance_x, canvas_distance_per_game_distance_y);
    }

    get_corner_nodes()
    {
        var corner1 = null;
        var corner2 = null;

        for(let node of this.nodes)
        {
            if(!node.corner)
                continue;
            
            if(corner1 == null)
                corner1 = node;
            else if(corner2 == null)
                corner2 = node;
        }
        return [corner1, corner2];
    }

    update_nodes_canvas_position_relative_to_reference_node(reference_node, canvas_distance_per_game_distance_x, canvas_distance_per_game_distance_y)
    {
        for(let node of this.nodes) 
        {
            if(reference_node.id == node.id)
                continue;

            var x = node.game_x - reference_node.game_x;
            var y = node.game_y - reference_node.game_y;

            node.canvas_x = reference_node.canvas_x + (x * canvas_distance_per_game_distance_x)
            node.canvas_y = reference_node.canvas_y - (y * canvas_distance_per_game_distance_y)
        }
    }
}
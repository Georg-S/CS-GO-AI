var POINT_BAR_WITDTH = 150;

class Node
{
    constructor(id, corner, game_x, game_y)
    {
        this.id = id
        this.corner = corner
        this.radius = 30;
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

    draw_nodes(nodes)
    {
        if(nodes == null || this.map == null)
            return;
        
        var count = 0;
        for(node of this.nodes) 
        {
            if(node["corner"] === true) 
            {
                node.canvas_x = this.canvas.width - (POINT_BAR_WITDTH / 2);
                node.canvas_y = 180 * count;
                this.draw_Node(node);
                count++;
            }
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
        const top_offset = 50;
        var context = this.canvas.getContext("2d");

        context.beginPath();
        context.arc(node.canvas_x, node.canvas_y + top_offset, node.radius, 0, 2 * Math.PI, false);
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
        context.fillText(text, node.canvas_x - diameter, node.canvas_y + diameter + y_offset * font_size + 50);
    }
}
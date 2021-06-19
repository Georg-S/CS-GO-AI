var POINT_BAR_WITDTH = 150;


class Canvas
{
    constructor()
    {
        this.canvas = document.getElementById("myCanvas");
        this.context = this.canvas.getContext("2d");
    }

    draw_line()
    {
        this.context.moveTo(0, 0);
        this.context.lineTo(200, 100);
        this.context.stroke();
    }

    draw_circle(y)
    {
        var radius = 30;
        var context = this.context

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
        this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
    }
}

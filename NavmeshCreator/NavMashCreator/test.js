class Render_Info 
{
    constructor(test) 
    {
        this.test = test
    }
}

render_info = new Render_Info(5)


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
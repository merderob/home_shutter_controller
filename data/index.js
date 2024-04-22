// Copyright © 2023 -2024 Robert Takacs
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

function sendAbsoluteCommand(form) 
{
    if (!form.living_room_door.checked && 
        !form.living_room_window.checked &&
        !form.bedroom_door.checked &&
        !form.bedroom_window.checked)
    {
        return;
    }
    form.action = "/get?shutter_scale=" + form.shutter_scale.value;
    if (form.living_room_door.checked)
    {
        form.action += ',living_room_door'
    }
    if (form.living_room_window.checked)
    {
        form.action += ',living_room_window'
    }
    if (form.bedroom_door.checked)
    {
        form.action += ',bedroom_door'
    }
    if (form.bedroom_window.checked)
    {
        form.action += ',bedroom_window'
    }
}

function calibrate(shutter_num) 
{
    const requestBody = {"please": shutter_num};
    const requestOptions = 
    {
        method: 'POST',
        headers: {'Content-Type': 'application/json', 
    },
        body: JSON.stringify(requestBody)
    };

    fetch("/api/calibrate", requestOptions)
    .then(res => res.json()).then( data => 
        {
            console.log(data);
            document.getElementById("cal_" + shutter_num).style.background='#000000';
        })
        .catch(function (err) 
        {
            console.log("Something went wrong!", err)
        });
}

if(typeof document != "undefined")
{
    function includeJs(jsFilePath) 
    {
        var js = document.createElement("script");
        js.type = "text/javascript";
        js.src = jsFilePath;
        document.body.appendChild(js);
    }
       
       includeJs("assets_preload.js");
}
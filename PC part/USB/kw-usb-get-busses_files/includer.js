function includeSyntax(name)
{
  if(typeof(window['highlightSyntax'])!="function")
  {
        document.write('<script src="/html/syntax_script/core.js"></script>');
  }
  if(typeof(window["setup_"+name])!="function")
  {
    document.write('<script src="/html/syntax_script/'+name+'.js"></script>');
  }
}

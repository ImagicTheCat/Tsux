window.onload = function(){
  var msg = $("#message");
  var msgs = $("#messages");
  var send = $("#send");

  function update(){
    $.get("/ajax/chat/list", function(data){
      var list = data.split("\n");
      for(var i = 0; i < list.length; i++){
        if(list[i] != "")
          msgs.append(list[i]+"<br />");
      }

      msgs.scrollTop(10000000);
    });
  }

  send.click(function(){
    $.post("/ajax/chat/send", {message: msg.val()}).done(function(data){
      msg.val("");
      update();
    });
  });

  update();
  setInterval(update, 2500);
}

//Open offsite links in new window script- http://www.dynamicdrive.com/
//Created: August 28th, 2007'

var ddwindowlinks={
//1)Enter domains to be EXCLUDED from opening in new window:
excludedomains: ["dynamicdrive.com", "google.com"],

//2) Target for links that should open in a new window (ie: "_blank", "secwin" etc):
linktarget: "_blank",

//3) Specify operating mode ("auto" or "manual"):
mode: "auto",

//4) If mode is "manual", customize checkbox HTML to show to users (Preserve id attribute):
toggleHTML: '<form><input type="checkbox" id="targetcheckbox" checked="checked" /><label for="targetcheckbox">Open off-site links in new window?</label></form>',

//5) If mode is "manual", enable user persistence so the state of the checkbox is remembered?
persist: true,

assigntarget:function(){
	var rexcludedomains=new RegExp(this.excludedomains.join("|"), "i")
	var all_links=document.getElementsByTagName("a")
	if (this.mode=="auto" || (this.mode=="manual" && this.togglebox.checked)){
		for (var i=0; i<=(all_links.length-1); i++){
			if (all_links[i].hostname.search(rexcludedomains)==-1 && all_links[i].href.indexOf("http:")!=-1)
				all_links[i].target=ddwindowlinks.linktarget
		}
	}
	else{
		for (var i=0; i<=(all_links.length-1); i++)
			all_links[i].target=""
	}
	if (this.mode=="manual" && this.persist)
		this.setCookie("dlinktarget", (this.togglebox.checked)? "yes" : "no", 30) //remember user setting for 30 days (set to -1 then reload page to erase cookie)
},

init:function(){
	if (document.getElementById && this.mode=="manual"){
		document.write(this.toggleHTML)
		this.togglebox=document.getElementById("targetcheckbox")
		this.togglebox.onclick=function(){ddwindowlinks.assigntarget()}
		if (this.persist && this.getCookie("dlinktarget")!="")
			this.togglebox.checked=(this.getCookie("dlinktarget")=="yes")? true : false
	}
	if (window.addEventListener)
		window.addEventListener("load", function(){ddwindowlinks.assigntarget()}, false)
	else if (window.attachEvent)
		window.attachEvent("onload", function(){ddwindowlinks.assigntarget()})
},

getCookie:function(Name){
	var re=new RegExp(Name+"=[^;]+", "i"); //construct RE to search for target name/value pair
	if (document.cookie.match(re)) //if cookie found
		return document.cookie.match(re)[0].split("=")[1] //return its value
	return ""
},

setCookie:function(name, value, days){
	var expireDate = new Date()
	//set "expstring" to either an explicit date (past or future)
		var expstring=expireDate.setDate(expireDate.getDate()+parseInt(days))
		document.cookie = name+"="+value+"; expires="+expireDate.toGMTString()+"; path=/"
}

}

ddwindowlinks.init()

!function(n,a,y,t,e,v,s){var globalNameSpace="GlobalNaytevNamespace";
n[e]||(n[globalNameSpace]=n[globalNameSpace]||[],n[globalNameSpace].push(e),n[e]=function(){(n[e].q=n[e].q||[]).push(arguments)
},n[e].q=n[e].q||[],v=a.createElement(y),s=a.getElementsByTagName(y)[0],v.async=1,v.src=t,s.parentNode.insertBefore(v,s))
}(window,document,"script","//naytev-embed.global.ssl.fastly.net/prod/naytev.min.js","naytev");
naytev("create", "54c80446e4b04014f615966f");
/*
    If you are reading this code, you are most definitely curious about the Version 1 Naytev embed. Guess what? It
    isn't here anymore and has upgraded to a faster, cleaner and more performant embed. There are two things you
    should do to move to the current version:
     1. Upgrade to the version 2 embed. See official docs for details: https://naytev.zendesk.com/hc/en-us/articles/204698048
     2. We are hiring! If you want to optimize the top sites in the world, send us an email at jobs@naytev.com

*/
var NV = window.NV || {};
NV.shareCount = function(options){
    naytev("share_count", options);
};
NV.activate = function(){
    naytev("activate");
};
window.NV = NV;


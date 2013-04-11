function calculeOffset(a,b){
  var c=0;
  while(a){
    c+=a[b];
    a=a.offsetParent
    }
    return c
  }
  function calculeOffsetTop(a){
  return calculeOffset(a,"offsetTop")
  }
  function calculeOffsetLeft(a){
  return calculeOffset(a,"offsetLeft")
  }
  var labels=new Array;
labels.bg=["bg","Споделяне","Затваряне","Споделяне","Електронно писмо","Предпочитани","Страница","Ако щракнете върху връзка към сайт на социална мрежа, ще напуснете уебсайта EUROPA и ще отидете на поддържан от трето лице сайт, който може да има политика на поверителност, различна от нашата.","Зареждане"];
labels.cs=["cs","Sdílet","Zavřít","Sdílet","Poslat e-mailem","Oblíbené","Strana","Pokud kliknete na odkaz na sociální síť či stránky pro sdílení záložek, opustíte portál EUROPA a dostanete se na jiné internetové stránky, jejichž ochrana osobních údajů se může lišit.","Probíhá načítání"];
labels.da=["da","Del","Luk","Del","E-mail","Favoritter","Side","Hvis du klikker på et socialt netværk eller en favorit, forlader du EUROPA-webstedet og går til et eksternt websted, som måske har en anden politik om beskyttelse af personoplysninger.","Loader"];
labels.de=["de","Weiterempfehlen","Schließen","Weiterempfehlen","E-Mail","Favoriten","Seite","Wenn Sie sich zu einem sozialen Netz oder einer webgestützten Favoritenliste durchklicken, verlassen Sie die Europa-Website und begeben sich auf eine externe Website, deren Datenschutzpolitik sich möglicherweise von der Datenschutzpolitik der EU unterscheidet.","Seite wird geladen"];
labels.et=["et","Jaga","Sulge","Jaga","E-post","Järjehoidjad","Lehekülg","Kui lähete sotsiaalsete võrgustike / järjehoidjates leiduvale leheküljele, lahkute sellega portaalist Europa. Muudel veebisaitidel võib isikuandmete kaitse poliitika olla erinev, kui portaalis Europa.","Laadimine"];
labels.el=["el","Διαδώστε το","Κλείσιμο","Διαδώστε το","Ηλεκτρονική διεύθυνση","Τα αγαπημένα","Σελίδα","Αν κάνετε κλικ σε δικτυακό τόπο κοινωνικού δικτύου / αγαπημένων σας ιστοσελίδων, εξέρχεστε από τον δικτυακό τόπο EUROPA και εισέρχεστε σε άλλο δικτυακό τόπο ο οποίος μπορεί να έχει διαφορετική πολιτική περί απορρήτου από τη δική μας.","Φόρτωση"];
labels.en=["en","Share","Close","Share","E-mail","Favorites","Page","If you click on a social network / bookmark site you will leave the EUROPA Website and go to a third party site which may have a different privacy policy from us.","Loading"];
labels.es=["es","Compartir","Cerrar","Compartir","Correo electrónico","Favoritos","Página","Al pulsar en una red social o servicio de marcadores abandonará EUROPA y se dirigirá al sitio de un tercero cuya política de privacidad puede diferir de la nuestra.","Cargando"];
labels.fr=["fr","Partager","Fermer","Partager","Courriel","Favoris","Page","Si vous cliquez sur l'icône d'un site de réseaux sociaux ou de gestion de favoris, vous quitterez le site EUROPA et accéderez à un site tiers dont la politique de protection des données personnelles pourrait différer de la nôtre.","Chargement en cours"];
labels.ga=["ga","Páirtigh ","Dún ","Páirtigh ","R-phost ","Ceanáin ","Leathanach","Má ghliogálann tú ar shuíomh líonra shóisialta / leabharmharcála fágfaidh tú suíomh gréasáin EUROPA is beidh tú ar shuíomh le tríú páirtí, is b'fhéidir nach ionann an beartas príobháídeachta againn.","Á lódáil"];
labels.it=["it","Condividi","Chiudi","Condividi","E-mail","Preferiti","Pagina","Se clicchi su un sito incluso fra i tuoi Preferiti o su una rete sociale, uscirai dal sito EUROPA ed entrerai in pagine che potrebbero avere norme sulla privacy diverse dalle nostre.","Caricamento"];
labels.lv=["lv","Kopīgot","Aizvērt","Kopīgot","E-pasts","Izlase","Lapa","Ja noklikšķināsit uz “Sociālais tīkls” vai “Pievienot vietni izlasei”, vairs nebūsit “Europa” tīmekļa vietnē, bet nonāksit citā, nesaistītā vietnē, kurā var tikt izmantota atšķirīga stratēģija attiecībā uz privātuma aizsardzību.","Ielāde"];
labels.lt=["lt","Siųsti draugams","Uždaryti","Siųsti draugams","E. pašto adresas","Parankiniai adresai","Puslapis","Spustelėję socialinio tinklo nuorodą ar žymelę, išeisite iš svetainės EUROPA ir pateksite į trečiųjų šalių svetainę, kurioje gali būti taikoma kitokia, nei mūsų, privatumo apsaugos politika.","Įkėlimas"];
labels.hu=["hu","Link ajánlása","Bezárás","Link ajánlása","E-mail cím","Kedvencek","Oldal","Ha Ön egy közösségi weboldal vagy egy elmentett oldal hivatkozására kattint, elhagyja az EUROPA portált, és harmadik fél webhelyére jut, melynek adatvédelmi szabályzata eltérhet a miénktől.","Betöltés"];
labels.mt=["mt","Aqsam","Agħlaq","Aqsam","Email","Favoriti ","Paġna","Jekk tikklikkja fuq sit ta' netwerk soċjali jew sit immarkat,  toħroġ mill-Websajt EUROPA u tidħol f'sit ta' terza parti li jista' jkollha politika ta' privatezza differenti minn tagħna.","Tiela l-paġna"];
labels.nl=["nl","Share","Sluiten","Share","E-mail","Favorieten","Pagina","Als u op een sociale netwerksite of bookmarksite klikt, verlaat u de EUROPA-website. Voor die andere sites kunnen andere privacyregels gelden.","Laden"];
labels.pl=["pl","Podziel się","Zamknij","Udostępnij","E-mail","Ulubione","Strona","Po kliknięciu na ikonkę serwisu społecznościowego lub zakładkę zostaniesz przekierowany z portalu EUROPA na strony zewnętrzne, które mogą mieć inną politykę prywatności niż nasza.","Ładowanie..."];
labels.pt=["pt","Partilhar","Fechar","Partilhar","Endereço electrónico","Favoritos","Página","Ao clicar numa rede social ou num serviço de marcação está a sair do portal EUROPA e a entrar noutro sítio Web que pode ter uma política de confidencialidade diferente da nossa.","A carregar"];
labels.ro=["ro","Partajează","Închide","Partajează","E-mail","Preferințe","Pagina","În momentul în care faceţi clic pe o platformă socială sau pe un site din lista de preferinţe, părăsiţi portalul EUROPA şi accesaţi site-uri care ar putea aplica o politică diferită în ceea ce priveşte protecţia datelor personale.","Încărcare"];
labels.sk=["sk","Zdieľať","Zavrieť","Zdieľať","E-mailová adresa","Obľúbené","Strana","Ak kliknete na stránku sociálnej siete / záložku, budete presmerovaný z webových stránok EUROPA na stránky tretích osôb, na ktorých sa môžu pravidlá o ochrane osobných údajov líšiť.","Načítava sa"];
labels.sl=["sl","Povej naprej","Zapri","Povej naprej","e-pošta","Priljubljeni","Stran","Če kliknete povezavo na družabno mrežo ali spletišče za posredovanje zaznamkov, boste zapustili portal EUROPA in odprli zunanjo spletno stran, ki ima morda drugačne pogoje varstva osebnih podatkov kot portal EUROPA.","Nalaganje"];
labels.fi=["fi","Jaa","Sulje","Jaa","Sähköposti","Suosikit","Sivu","Jos siirryt Europa-sivustolta jonkin nettiyhteisön sivustolle tai kirjanmerkillä merkitylle sivustolle, huomaa, että sen tietosuojaperiaatteet voivat poiketa Europa-sivustosta.","Ladataan"];
labels.sv=["sv","Dela med dig","Stäng","Dela med dig","E-post","Favoriter","Sida","När du klickar på ett socialt nätverk eller en bokmärkeswebbplats lämnar du Europa-portalen och kommer till någon annans webbplats, där man eventuellt har andra regler för skydd av personuppgifter än vi.","Laddar"];
var iBeginShare=function(){
  var a={
    base_url:"./",
    base_url_stats:"http://webtools.ec.europa.eu/socialbookmark/",
    default_skin:"default",
    default_link:"button",
    default_link_skin:"default",
    default_share_style:"button",
    default_share_size:"16",
    default_share_text:"",
    default_share_style_link:"",
    script_handler:false,
    close_label:"x",
    text_link_label:"Share",
    tab_index_start:100,
    version_number:"1.0.14",
    button_id:"",
    count_open:0,
    is_opera:navigator.userAgent.indexOf("Opera/9")!=-1,
    is_ie:navigator.userAgent.indexOf("MSIE ")!=-1,
    is_safari:navigator.userAgent.indexOf("webkit")!=-1,
    needs_iframe_shime:false,
    is_firefox:navigator.appName=="Netscape"&&navigator.userAgent.indexOf("Gecko")!=-1&&navigator.userAgent.indexOf("Netscape")==-1,
    is_mac:navigator.userAgent.indexOf("Macintosh")!=-1,
    http:null,
    default_lang:"en",
    all_lang:"bg,cs,da,de,el,en,es,et,fi,fr,ga,hu,it,lt,lv,mt,nl,pl,pt,ro,sk,sl,sv",
    getMetaValue:function(a){
      var b=document.getElementsByTagName("meta");
      var c;
      var d="";
      var e;
      var f;
      for(var g=0;g<b.length;g++){
        if(1==b[g].nodeType){
          c=b[g].attributes;
          e="";
          f="";
          for(var h=0;h<c.length;h++){
            if(""!=c[h].value&&("name"==c[h].name||"http-equiv"==c[h].name)){
              e=c[h].value
              }else{
              if("content"==c[h].name){
                f=c[h].value
                }
              }
          }
          if(e.toLowerCase()==a.toLowerCase()){
        d=f;
        break
      }
      }
    }
  return d.toLowerCase()
},
needsIframeShime:function(){
  var a=false;
  var b=-1;
  if(navigator.appName=="Microsoft Internet Explorer"){
    var c=navigator.userAgent;
    var d=new RegExp("MSIE ([0-9]{1,}[.0-9]{0,})");
    if(d.exec(c)!=null){
      b=parseFloat(RegExp.$1)
      }
      a=b>=5.5&&b<=6
    }else{
    if(navigator.appName=="Opera"){
      var c=navigator.userAgent;
      var d=new RegExp("Opera/([0-9]{1,}[.0-9]{0,})");
      if(d.exec(c)!=null){
        b=parseFloat(RegExp.$1)
        }
        a=b>=9
      }
    }
  return a
},
getLangFromURL:function(a,b,c){
  var d="";
  var e=new RegExp(b,"gi");
  var f;
  f=e.exec(a);
  if(f!=null){
    if(f[c]!=null){
      d=f[c]
      }
    }
  return d.toLowerCase()
},
getLang:function(b){
  if(b.toLowerCase()=="frommeta"){
    var c=a.getMetaValue("content-language");
    if(c!=""){
      a.default_lang=c
      }
    }else{
  if(b.toLowerCase()=="fromurl"){
    var d=document.URL;
    var e=d.indexOf("?");
    if(e<=0){
      var f=d
      }else{
      var f=d.substr(0,e)
      }
      var g=a.getLangFromURL(f,"(_|-)([a-zA-Z]{2}).(html?|php|asp|cgi|jsp|cfm)",2);
    if(g!=""){
      a.default_lang=g
      }
    }else{
  if(a.all_lang.indexOf(b)>-1){
    a.default_lang=b
    }
  }
}
if(a.all_lang.indexOf(a.default_lang)<0){
  a.default_lang="en"
  }
},
addToFavorites:function(a,b){
  a=decodeURIComponent(a);
  b=decodeURIComponent(b);
  if(window.sidebar){
    window.sidebar.addPanel(b,a,"")
    }else{
    if(window.external){
      window.external.AddFavorite(a,b)
      }else{
      if(window.opera&&window.print){};

  }
}
return true
},
printPage:function(){
  self.print()
  },
include:function(a){
  var b=document.getElementsByTagName("head")[0];
  var c=document.createElement("link");
  c.href=a;
  if(a.substr(a.length-9)=="print.css"){
    c.media="print"
    }else{
    c.media="screen"
    }
    c.rel="stylesheet";
  c.type="text/css";
  b.appendChild(c)
  },
discoverBaseUrl:function(){
  var b=document.getElementsByTagName("script");
  var c;
  for(var d=0,e=null;e=b[d];d++){
    if(!(c=e.getAttribute("src"))){
      continue
    }
    c=c.split("?")[0];
    if(c.substr(c.length-9)=="/share.js"){
      a.base_url=c.substr(0,c.length-8);
      break
    }
  }
  },
enableStats:function(){
  a.script_handler=a.base_url_stats+"share.php?action=log"
  },
createElement:function(b,c){
  var d=document.createElement(b);
  if(!c){
    return d
    }
    for(var e in c){
    if(e=="className"){
      d.className=c[e]
      }else{
      if(e=="text"){
        d.appendChild(document.createTextNode(c[e]))
        }else{
        if(e=="html"){
          d.innerHTML=c[e]
          }else{
          if(e=="id"){
            d.id=c[e]
            }else{
            if(e=="children"){
              continue
            }else{
              if(e=="events"){
                for(var f in c[e]){
                  if(c[e][f]!=null){
                    a.addEvent(d,f,c[e][f])
                    }
                  }
                }else{
            if(e=="styles"){
              for(var f in c[e]){
                d.style[f]=c[e][f]
                }
              }else{
            if(e=="htmlFor"){
              d.setAttribute("for",c[e])
              }else{
              d.setAttribute(e,c[e])
              }
            }
        }
    }
  }
}
}
}
}
if(c.children){
  for(var g=0;g<c.children.length;g++){
    d.appendChild(c.children[g])
    }
  }
  return d
},
parseQuery:function(a){
  var b=new Object;
  if(!a){
    return b
    }
    var c=a.split(/[;&]/);
  var d;
  for(var e=0;e<c.length;e++){
    var f=c[e].split("=");
    if(!f||f.length!=2){
      continue
    }
    var g=unescape(f[0]);
    var h=unescape(f[1]);
    h=h.replace(/\+/g," ");
    if(h[0]=='"'){
      var i='"'
      }else{
      if(h[0]=="'"){
        var i="'"
        }else{
        var i=null
        }
      }
    if(i){
    if(h[h.length-1]!=i){
      do{
        e+=1;
        h+="&"+c[e]
        }while((d=c[e][c[e].length-1])!=i)
    }
    h=h.substr(1,h.length-2)
    }
    if(h=="true"){
    h=true
    }else{
    if(h=="false"){
      h=false
      }else{
      if(h=="null"){
        h=null
        }
      }
  }
  b[g]=h
}
return b
},
serializeFormData:function(a){
  var b={};

  var c=a.getElementsByTagName("input");
  for(var d=0,e=null;e=c[d];d++){
    if(e.name){
      if(e.type=="text"||e.type=="hidden"||e.type=="password"||(e.type=="radio"||e.type=="checkbox")&&e.checked){
        b[e.name]=encodeURIComponent(e.value)
        }
      }
  }
  var c=a.getElementsByTagName("textarea");
for(var d=0,e=null;e=c[d];d++){
  if(e.name){
    b[e.name]=encodeURIComponent(e.value)
    }
  }
var c=a.getElementsByTagName("select");
for(var d=0,e=null;e=c[d];d++){
  if(e.name){
    b[e.name]=encodeURIComponent(e[e.selectedIndex].value)
    }
  }
return b
},
makeSafeString:function(a){
  return a.replace(/[^a-zA-Z0-9_-s.]/,"").toLowerCase()
  },
createParametersString:function(a){
  var b="";
  for(var c in a){
    if(typeof a[c]=="object"){
      for(var d=0;d<a[c].length;d++){
        b+=c+"="+encodeURIComponent(a[c][d])+"&"
        }
      }else{
    b+=c+"="+encodeURIComponent(a[c])+"&"
    }
  }
  return b
},
showLoadingBar:function(){
  b.loading.className="share_loading share_display_block";
  b.content_inner.className="share_display_none"
  },
hideLoadingBar:function(){
  b.loading.className="share_loading share_display_none";
  b.content_inner.className="share_display_block"
  },
hasClass:function(a,b){
  if(a.className){
    var c=a.className.split(" ");
    var d=b.toUpperCase();
    for(var e=0;e<c.length;e++){
      if(c[e].toUpperCase()==d){
        return true
        }
      }
    }
  return false
},
toggleClass:function(b,c){
  if(a.hasClass(b,c)){
    a.removeClass(b,c)
    }else{
    a.addClass(b,c)
    }
  },
addClass:function(a,b){
  a.className=a.className?a.className+" "+b:b
  },
removeClass:function(a,b){
  if(a.className){
    var c=a.className.split(" ");
    var d=b.toUpperCase();
    for(var e=0;e<c.length;e++){
      if(c[e].toUpperCase()==d){
        c.splice(e,1);
        e--
      }
    }
    a.className=c.join(" ")
  }
},
empty:function(a){
  while(a.firstChild){
    a.removeChild(a.firstChild)
    }
  },
html:function(c){
  if(!c){
    return
  }
  a.hideLoadingBar();
  a.empty(b.content_inner);
  if(typeof c=="string"){
    b.content_inner.innerHTML=c
    }else{
    b.content_inner.appendChild(c)
    }
  },
hide:function(){
  if(c.tab&&c.tab.plugin.unload){
    c.tab.plugin.unload()
    }
    if(c.link){
    a.removeClass(c.link,"share_button_active")
    }
    c={};

  b.box.style.display="none";
  if(this.needs_iframe_shime){
    var d=document.getElementById("share_shimmer");
    if(d){
      document.body.removeChild(d)
      }
    }
    if(a.button_id) {
      var e=document.getElementById(a.button_id).getElementsByTagName("div")[0].getElementsByTagName("a")[0];
    }
if(e){
  e.tabIndex=0;
  e.removeAttribute("tabIndex");
  e.focus()
  }
},
show:function(d,e){
  var f=a.button_id;
  a.button_id=d.parentNode.parentNode.id;
  if(f!=""&&f!=a.button_id){
    var g=document.getElementById(f).getElementsByTagName("div")[0].getElementsByTagName("a")[0];
    if(g){
      g.tabIndex=0;
      g.removeAttribute("tabIndex")
      }
    }
  if(!a.plugins.list.length){
  return false
  }
  if(c.link!==undefined&&c.link==d){
  return false
  }
  if(e===undefined){
  e={}
}
if(!e.link){
  e.link=window.location.href
  }
  if(!e.title){
  e.title=document.title
  }
  if(!e.skin){
  e.skin=a.default_skin
  }
  if(c.link){
  a.hide()
  }
  c.link=d;
c.link.params=e;
a.addClass(d,"share_button_active");
if(!e.skin){
  e.skin="default"
  }
  b.box.style.position="absolute";
b.box.style.display="block";
b.box.style.visibility="hidden";
b.box.style.top=0;
b.box.style.left=0;
if(e.share_size===undefined){
  e.share_size=a.default_share_size
  }
  var h=0;
var i=0;
var j;
var k=0;
var l=0;
var m=0;
var n=0;
if(document.documentElement&&document.documentElement.scrollTop){
  m=document.documentElement.scrollTop
  }else{
  if(document.body&&document.body.scrollTop){
    m=document.body.scrollTop
    }
  }
if(document.documentElement&&document.documentElement.scrollLeft){
  n=document.documentElement.scrollLeft
  }else{
  if(document.body&&document.body.scrollLeft){
    n=document.body.scrollLeft
    }
  }
h+=m;
i+=n;
if(d.getBoundingClientRect){
  var o=d.getBoundingClientRect();
  i+=o.left;
  k=o.left;
  h+=o.top;
  l=o.top
  }else{
  if(d.offsetParent){
    do{
      if(a.getStyle(d,"position")=="relative"){
        if(j=a.getStyle(d,"border-top-width")){
          h+=parseInt(j)
          }
          if(j=a.getStyle(d,"border-left-width")){
          i+=parseInt(j)
          }
        }else{
      if(d.currentStyle&&d.currentStyle.hasLayout&&d!==document.body){
        i+=d.clientLeft;
        h+=d.clientTop
        }
      }
    h+=d.offsetTop;
  i+=d.offsetLeft
  }while(d=d.offsetParent)
}else{
  if(d.x){
    h+=d.y;
    i+=d.x
    }
  }
}
b.box.style.visibility="visible";
for(var p=0;p<a.plugins.list.length;p++){
  var q=a.plugins.list[p];
  var r=true;
  if(q.requires){
    for(var s=0;s<q.requires.length;s++){
      if(!e||!e[q.requires[s]]){
        r=false;
        break
      }
    }
    }
  q.tab.className="share_display_none"
}
c.tab=a.plugins.list[0].tab;
c.tab.className="share_display_block";
c.tab.plugin.render(a.showPlugin,e);
var t=a.getPageSize();
if(i+b.box.offsetWidth>t.width+n&&i>b.box.offsetWidth){
  i=n+t.width-b.box.offsetWidth-16;
  if(a.is_ie){
    i+=16
    }
  }else{}
if(h+b.box.offsetHeight>t.height+m&&h>b.box.offsetHeight){
  h=m+l-b.box.offsetHeight-2;
  if(a.is_ie){
    h-=2
    }
  }else{
  h=m+l+parseInt(e.share_size)
  }
  var u=document.getElementById("share_button_link");
var v=calculeOffsetTop(u);
var w=calculeOffsetLeft(u);
if(b.box.offsetHeight+m<=v)b.box.style.top=v-b.box.offsetHeight-1+"px";else b.box.style.top=v+u.offsetHeight+1+"px";
b.box.style.left=w+"px";
this.needs_iframe_shime=this.needsIframeShime();
if(this.needs_iframe_shime){
  var x=document.createElement("iframe");
  x.id="share_shimmer";
  x.style.position="absolute";
  x.style.width=b.box.offsetWidth+"px";
  x.style.height=b.box.offsetHeight+"px";
  x.style.top=h;
  x.style.left=i;
  x.style.filter="progid:DXImageTransform.Microsoft.Alpha(style=0,opacity=0)";
  x.style.backgroundColor="#f2f2f2";
  x.style.zIndex="10";
  x.setAttribute("frameborder","0");
  x.setAttribute("src","");
  document.body.appendChild(x)
  }
  var y=document.getElementById(a.button_id).getElementsByTagName("div")[0].getElementsByTagName("a")[0];
if(y){
  y.tabIndex=a.tab_index_start-1
  }
  var z=document.getElementById("share_icon_email_link_id");
if(z&&!this.needs_iframe_shime){
  z.focus()
  }
  if(y){
  y.focus()
  }
  return true
},
getPageSize:function(){
  return{
    width:window.innerWidth||document.documentElement&&document.documentElement.clientWidth||document.body.clientWidth,
    height:window.innerHeight||document.documentElement&&document.documentElement.clientHeight||document.body.clientHeight
    }
  },
showPlugin:function(b,c){
  a.html(b)
  },
handleLink:function(b){
  if(!b){
    b=window.event
    }
    var c=b.target?b.target:b.srcElement;
  if(b.preventDefault){
    b.preventDefault()
    }
    var d=c.params?c.params:a.parseQuery(c.getAttribute("rel"));
  if(a.hasClass(c,"share_button_active")){
    iBeginShare.hide(c)
    }else{
    iBeginShare.show(c,d)
    }
  },
drawLink:function(b,c){
  if(c===undefined){
    c={}
  }
  if(c.link_style===undefined){
  c.link_style=a.default_link
  }
  if(c.link_skin===undefined){
  c.link_skin=a.default_link_skin
  }
  if(c.link_label===undefined){
  c.link_label=a.text_link_label
  }
  if(c.share_style===undefined){
  c.share_style=a.default_share_style
  }
  if(c.share_size===undefined){
  c.share_size=a.default_share_size
  }
  if(c.share_text===undefined){
  c.share_text=labels[a.default_lang][1]
  }
  if(c.share_style_link===undefined){
  c.share_style_link=a.default_share_style_link
  }
  if(c.share_text==""){
  c.share_text=" ";
  c.share_style_link=" share_style_link"
  }
  var d=a.createElement("a",{
  href:"javascript:void(0)",
  id:"share_button_link",
  html:c.share_text,
  events:{
    click:a.handleLink
    }
  });
d.params=c;
b.appendChild(a.createElement("div",{
  id:"share_button_container",
  className:"share_button share_style_"+c.share_style+"_"+c.share_size+" share_size_"+c.share_size+c.share_style_link,
  children:[d]
  }))
},
drawButton:function(b,c){
  c.link_style="button";
  a.drawLink(b,c)
  },
drawTextLink:function(b,c){
  c.link_style="text";
  a.drawLink(b,c)
  },
attachLink:function(b,c){
  if(c===undefined){
    c={}
  }
  if(c.share_lang===undefined||c.share_lang==""){
  a.getLang("frommeta")
  }else{
  a.getLang(c.share_lang)
  }
  if(c.share_stats===undefined||c.share_stats==true){
  a.enableStats()
  }
  if(typeof b=="string"){
  b=document.getElementById(b)
  }
  a.addEvent(window,"load",a.bind(function(a,b,c){
  iBeginShare.drawLink(b,c)
  },b,c))
},
attachButton:function(b,c){
  c.link_style="button";
  a.attachLink(b,c)
  },
attachTextLink:function(b,c){
  c.link_style="text";
  a.attachLink(b,c)
  },
bind:function(a){
  var b=[];
  for(var c=1;c<arguments.length;c++){
    b.push(arguments[c])
    }
    return function(c){
    return a.apply(this,[c].concat(b))
    }
  },
addEvent:function(a,b,c){
  if(a.addEventListener){
    a.addEventListener(b,c,false);
    return true
    }else{
    if(a.attachEvent){
      var d=a.attachEvent("on"+b,c);
      return d
      }else{
      return false
      }
    }
},
getStyle:function(a,b){
  var c;
  if(a.currentStyle){
    c=a.currentStyle[b]
    }else{
    if(window.getComputedStyle){
      c=document.defaultView.getComputedStyle(a,null).getPropertyValue(b)
      }
    }
  return c
},
getContainer:function(){
  return b.box
  },
makeLoggableUrl:function(b,e,f,g){
  if(!a.script_handler){
    if(e.toLowerCase()=="wordpress"){
      return g
      }else{
      return g.replace("__URL__",b).replace("__TITLE__",f)
      }
    }
  if(e===undefined){
  e=""
  }
  var h=c.tab.plugin.log_key;
if(!h){
  var h=a.makeSafeString(c.tab.plugin.label)
  }
  return a.script_handler+"&plugin="+encodeURIComponent(h)+"&name="+encodeURIComponent(e)+"&link="+encodeURIComponent(b)+"&title="+encodeURIComponent(f)+"&link_bookmark="+encodeURIComponent(g)+"&"+d()
},
plugins:{
  builtin:{
    bookmarks:function(){
      var c=2;
      var d=7;
      var e;
      var f;
      var g;
      var h=new Array;
      var i=function(a){
        if(!a){
          a=window.event
          }
          var b=a.target?a.target:a.srcElement;
        j(b.getAttribute("rel"));
        if(a.preventDefault){
          a.preventDefault()
          }
          return false
        };

      var j=function(b){
        if(e==b){
          return
        }
        var i=document.getElementById("bm_page_"+e);
        if(i){
          i.className=""
          }
          var j=k.getElementsByTagName("div")[0];
        a.empty(j);
        var q=b*d*c;
        var r=q-d*c;
        var s=a.createElement("div",{
          className:"share_column"
        });
        var t=a.createElement("ul",{
          className:"share_list"
        });
        var u;
        if(b==1){
          u=a.createElement("li");
          u.appendChild(a.createElement("a",{
            className:"share_icon_email_link",
            id:"share_icon_email_link_id",
            title:labels[a.default_lang][4],
            target:"_blank",
            href:h[0][1].replace("__URL__",f).replace("__TITLE__",g),
            tabIndex:a.tab_index_start,
            html:labels[a.default_lang][4],
            events:{
              click:function(){
                var b=a.createElement("img",{
                  src:a.makeLoggableUrl(f,"E-mail",g,h[0][1]),
                  className:"share_display_none"
                });
                j.appendChild(b);
                return true
                }
              }
          }));
      t.appendChild(u)
      }
      for(var v=r;v<q;v+=2){
      if(!h[v]){
        break
      }
      if(h[v][0]=="Favorites"||h[v][0]=="Email_link"){
        continue
      }
      if(h[v][0].toLowerCase()=="wordpress"){
        var w=h[v][1];
        u=a.createElement("li");
        u.appendChild(a.createElement("a",{
          className:"share_icon_"+h[v][0].toLowerCase(),
          title:h[v][0],
          href:"javascript:void(0);",
          tabIndex:a.tab_index_start+v,
          html:h[v][0],
          events:{
            click:function(){
              var b=w.replace("__URL__",f).replace("__TITLE__",g).replace("__ID__",prompt("Wordpress ID?",""));
              window.open(b,"_blank");
              var c=a.createElement("img",{
                src:a.makeLoggableUrl(f,"WordPress",g,b),
                className:"share_display_none"
              });
              j.appendChild(c)
              }
            }
        }));
    t.appendChild(u);
      continue
    }
    if(h[v][0].toLowerCase()==""){
      u=a.createElement("li");
      u.appendChild(a.createElement("span",{
        className:"share_icon_"+h[v][0].toLowerCase(),
        html:" "
      }));
      t.appendChild(u);
      continue
    }
    u=a.createElement("li");
    u.appendChild(a.createElement("a",{
      className:"share_icon_"+h[v][0].toLowerCase(),
      title:h[v][0],
      target:"_blank",
      href:a.makeLoggableUrl(f,h[v][0],g,h[v][1]),
      tabIndex:a.tab_index_start+v,
      html:h[v][0]
      }));
    t.appendChild(u)
    }
    s.appendChild(t);
j.appendChild(s);
s=a.createElement("div",{
  className:"share_column"
});
var t=a.createElement("ul",{
  className:"share_list"
});
if(b==1){
  u=a.createElement("li");
  u.appendChild(a.createElement("a",{
    className:"share_icon_favorites",
    title:labels[a.default_lang][5],
    href:"javascript:void(0);",
    tabIndex:a.tab_index_start+1,
    html:labels[a.default_lang][5],
    events:{
      click:function(){
        a.addToFavorites(f,g);
        var b=a.createElement("img",{
          src:a.makeLoggableUrl(f,"Favorites","Favorites",h[v][1]),
          className:"share_display_none"
        });
        j.appendChild(b);
        return true
        }
      }
  }));
t.appendChild(u)
}
for(var v=r+1;v<q;v+=2){
  if(!h[v]){
    break
  }
  if(h[v][0]=="Favorites"||h[v][0]=="Email_link"){
    continue
  }
  if(h[v][0].toLowerCase()=="wordpress"){
    var w=h[v][1];
    u=a.createElement("li");
    u.appendChild(a.createElement("a",{
      className:"share_icon_"+h[v][0].toLowerCase(),
      title:h[v][0],
      href:"javascript:void(0);",
      tabIndex:a.tab_index_start+v,
      html:h[v][0],
      events:{
        click:function(){
          var b=w.replace("__URL__",f).replace("__TITLE__",g).replace("__ID__",prompt("Wordpress ID?",""));
          window.open(b,"_blank");
          var c=a.createElement("img",{
            src:a.makeLoggableUrl(f,"WordPress",g,b),
            className:"share_display_none"
          });
          j.appendChild(c)
          }
        }
    }));
t.appendChild(u);
  continue
}
if(h[v][0].toLowerCase()==""){
  u=a.createElement("li");
  u.appendChild(a.createElement("span",{
    className:"share_icon_"+h[v][0].toLowerCase(),
    html:" "
  }));
  t.appendChild(u);
  continue
}
u=a.createElement("li");
u.appendChild(a.createElement("a",{
  className:"share_icon_"+h[v][0].toLowerCase(),
  title:h[v][0],
  target:"_blank",
  href:a.makeLoggableUrl(f,h[v][0],g,h[v][1]),
  tabIndex:a.tab_index_start+v,
  html:h[v][0]
  }));
t.appendChild(u)
}
s.appendChild(t);
j.appendChild(s);
e=b;
var i=document.getElementById("bm_page_"+e);
if(i){
  i.className="active"
  }
};

var k=null;
return{
  log_key:"bookmarks",
  label:"bookmarks",
  requires:["link","title"],
  addService:function(a,b){
    h.push([a,b])
    },
  render:function(q,r){
    e=null;
    f=encodeURIComponent(r.link);
    g=encodeURIComponent(r.title);
    var s=Math.ceil(h.length/(d*c));
    k=a.createElement("div",{
      id:"share_networks",
      children:[a.createElement("div",{
        id:"share_networks_list"
      })]
      });
    if(s>1){
      var t=new Array;
      for(var u=1;u<=s;u++){
        t.push(a.createElement("a",{
          id:"bm_page_"+u,
          html:u,
          href:"javascript:void(0);",
          title:labels[a.default_lang][6]+" "+u,
          className:u==1?"active":"",
          rel:u,
          tabIndex:a.tab_index_start+u+s*d*c,
          events:{
            click:i
          }
        }))
      }
      k.appendChild(a.createElement("div",{
      className:"share_pages",
      children:t
    }))
    }
    b.footer=a.createElement("div",{
    className:"share_footer",
    html:labels[a.default_lang][7]
    });
  k.appendChild(b.footer);
  j(1);
  q(k,r)
  }
}
}()
},
list:new Array,
register:function(){
  for(var b=0;b<arguments.length;b++){
    a.plugins.list.push(arguments[b]);
    f(arguments[b])
    }
    return true
  },
unregister:function(){
  var b=new Array;
  var c=new Array;
  for(var d=0;d<arguments.length;d++){
    c.push(arguments[d])
    }
    for(var d=0;d<a.plugins.list.length;d++){
    var e=false;
    for(var f=0;f<c.length;f++){
      if(a.plugins.list[d]==c[f]){
        e=true
        }
      }
    if(!e){
    b.push(a.plugins.list[d])
    }
  }
  if(a.plugins.list.length==b.length){
  return false
  }
  a.plugins.list=b;
return true
}
}
};

var b={};

var c={};

var d=function(){
  return Math.floor(Math.random()*10000001)
  };

var e=function(){
  b.box=a.createElement("div",{
    className:"share_box share_display_none",
    id:"share_box",
    children:[a.createElement("div",{
      title:labels[a.default_lang][2],
      id:"share_close",
      className:"share_button_close",
      events:{
        click:function(a){
          iBeginShare.hide();
          return false
          }
        },
    children:[a.createElement("a",{
      title:labels[a.default_lang][2],
      id:"share_close_link",
      text:" ",
      href:"javascript:void(0);",
      tabIndex:a.tab_index_start+110
      })]
    })]
  });
b.content=a.createElement("div",{
  className:"share_content"
});
b.title=a.createElement("div",{
  className:"share_header",
  html:""
});
b.content.appendChild(b.title);
for(var c=0;c<a.plugins.list.length;c++){
  f(a.plugins.list[c])
  }
  b.loading=a.createElement("div",{
  className:"share_loading share_display_block",
  html:labels[a.default_lang][8]
  });
b.content.appendChild(b.loading);
b.content_inner=a.createElement("div",{
  id:"share_content_inner"
});
b.content.appendChild(b.content_inner);
b.box.appendChild(b.content);
document.body.appendChild(b.box);
return b.box
};

var f=function(d){
  if(!b.box){
    return
  }
  var e=a.createElement("span",{
    id:"share_tab_"+d.label,
    html:labels[a.default_lang][3]
    });
  e.plugin=d;
  d.tab=e;
  e.onclick=function(b){
    if(c.tab==e){
      return false
      }
      a.showLoadingBar();
    if(c.tab.plugin.unload){
      c.tab.plugin.unload()
      }
      c.tab.className="share_display_none";
    c.tab=e;
    c.tab.className="share_display_block";
    d.render(a.showPlugin,c.link.params);
    return false
    };

  b.title.appendChild(e)
  };

var g=function(){
  e();
  document.body.style.position="relative"
  };

a.discoverBaseUrl();
a.include(a.base_url+"share.css");
a.include(a.base_url+"share-print.css");
a.addEvent(window,"load",g);
a.addEvent(window,"keypress",function(a){
  var b;
  if(!a){
    a=window.event
    }
    b=a?27:a.DOM_VK_ESCAPE;
  if(a.keyCode==b){
    iBeginShare.hide()
    }
  });
return a
}();
iBeginShare.plugins.builtin.bookmarks.addService("Email_link","mailto:?subject=%20&body=__URL__%20");
iBeginShare.plugins.builtin.bookmarks.addService("Favorites","");
iBeginShare.plugins.builtin.bookmarks.addService("MySpace","http://www.myspace.com/Modules/PostTo/Pages/?u=__URL__&t=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Facebook","http://www.facebook.com/share.php?u=__URL__&t=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Twitter","http://twitter.com/home?status=__TITLE__%20-%20__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Delicious","http://delicious.com/post?url=__URL__&title=__TITLE__&notes=");
iBeginShare.plugins.builtin.bookmarks.addService("Digg","http://digg.com/submit?phase=2&url=__URL__&title=__TITLE__&bodytext=");
iBeginShare.plugins.builtin.bookmarks.addService("Technorati","http://technorati.com/faves?sub=favthis&add=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Reddit","http://reddit.com/submit?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Bebo","http://bebo.com/c/share?Url=__URL__&Title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Blogger","http://www.blogger.com/blog_this.pyra?t=__TITLE__&u=__URL__&n=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Google","http://www.google.com/bookmarks/mark?op=edit&bkmk=__URL__&title=__TITLE__&annotation=");
iBeginShare.plugins.builtin.bookmarks.addService("Live","https://favorites.live.com/quickadd.aspx?marklet=1&url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("StumbleUpon","http://www.stumbleupon.com/submit?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Netlog","http://www.netlog.com/go/manage/links/?view=save&origin=external&url=__URL__&title=__TITLE__&description=");
iBeginShare.plugins.builtin.bookmarks.addService("Typepad","http://www.typepad.com/services/quickpost/post?v=2&qp_show=ac&qp_title=__TITLE__&qp_href=__URL__&qp_text=");
iBeginShare.plugins.builtin.bookmarks.addService("LinkedIn","http://www.linkedin.com/shareArticle?mini=true&url=__URL__&title=__TITLE__&ro=false&summary=&source=");
iBeginShare.plugins.builtin.bookmarks.addService("Netvibes","http://www.netvibes.com/share?title=__TITLE__&url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("YahooBuzz","http://buzz.yahoo.com/buzz?src=ec.europa.eu&targetUrl=__URL__&headline=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Yahoo","http://bookmarks.yahoo.com/toolbar/savebm?u=__URL__&t=__TITLE__&opener=bm&ei=UTF-8&d=");
iBeginShare.plugins.builtin.bookmarks.addService("AIMShare","http://connect.aim.com/share/?url=__URL__&title=__TITLE__&description=");
iBeginShare.plugins.builtin.bookmarks.addService("Viadeo","http://www.viadeo.com/shareit/share/?url=__URL__&title=__TITLE__&encoding=UTF-8");
iBeginShare.plugins.builtin.bookmarks.addService("Ask","http://myjeeves.ask.com/mysearch/BookmarkIt?v=1.2&t=webpages&url=__URL__&title=__TITLE__&abstext=");
iBeginShare.plugins.builtin.bookmarks.addService("WordPress","http://__ID__.wordpress.com/wp-admin/press-this.php?u=__URL__&t=__TITLE__&s=&v=2");
iBeginShare.plugins.builtin.bookmarks.addService("Gmail","https://mail.google.com/mail/?view=cm&fs=1&tf=1&to=&su=__TITLE__&body=__URL__&zx=RANDOMCRAP&shva=1&disablechatbrowsercheck=1&ui=1");
iBeginShare.plugins.builtin.bookmarks.addService("Mixx","http://www.mixx.com/submit?page_url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Arto","http://www.arto.com/section/linkshare/?lu=__URL__&ln=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Bitly","http://bit.ly/?url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("BlinkList","http://www.blinklist.com/index.php?Action=Blink/addblink.php&Url=__URL__&Title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Bloggy","http://bloggy.se/home?status=__TITLE__ __URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Blogmarks","http://blogmarks.net/my/new.php?mini=1&simple=1&url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Blogter","http://cimlap.blogter.hu/index.php?action=suggest_link&title=__TITLE__&url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Bobrdobr","http://bobrdobr.ru/addext.html?url=__URL__&title=__TITLE__&desc=");
iBeginShare.plugins.builtin.bookmarks.addService("Connotea","http://www.connotea.org/add?uri=__URL__&title=__TITLE__&description=");
iBeginShare.plugins.builtin.bookmarks.addService("Current","http://current.com/clipper.htm?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Diigo","http://www.diigo.com/post?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("DotNetKicks","http://www.dotnetkicks.com/kick/?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("DZone","http://www.dzone.com/links/add.html?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("eKudos","http://www.ekudos.nl/artikel/nieuw?url=__URL__&title=__TITLE__&desc=");
iBeginShare.plugins.builtin.bookmarks.addService("Fark","http://cgi.fark.com/cgi/fark/farkit.pl?h=__TITLE__&u=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Faves","http://faves.com/Authoring.aspx?u=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("FriendFeed","http://www.friendfeed.com/share?title=__TITLE__&link=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("GlobalGrind","http://globalgrind.com/submission/submit.aspx?url=__URL__&type=Article&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Gwar","http://www.gwar.pl/DodajGwar.html?u=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("HackerNews","http://news.ycombinator.com/submitlink?u=__URL__&t=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Haohao","http://www.haohaoreport.com/submit.php?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("HelloTxt","http://hellotxt.com/?status=__TITLE__+__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Hemidemi","http://www.hemidemi.com/user_bookmark/new?title=__TITLE__&url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Hyves","http://www.hyves.nl/profilemanage/add/tips/?name=__TITLE__&text=+__URL__&rating=5");
iBeginShare.plugins.builtin.bookmarks.addService("IdentiCa","http://identi.ca/notice/new?status_textarea=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("IndianPad","http://www.indianpad.com/submit.php?url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Kirtsy","http://www.kirtsy.com/submit.php?url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("LaaikIT","http://laaik.it/NewStoryCompact.aspx?uri=__URL__&headline=__TITLE__&cat=5e082fcc-8a3b-47e2-acec-fdf64ff19d12");
iBeginShare.plugins.builtin.bookmarks.addService("LinkaGoGo","http://www.linkagogo.com/go/AddNoPopup?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("LinkArena","http://linkarena.com/bookmarks/addlink/?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Linkter","http://www.linkter.hu/index.php?action=suggest_link&url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Linkuj","http://linkuj.cz/?id=linkuj&url=__URL__&title=__TITLE__&description=&imgsrc=");
iBeginShare.plugins.builtin.bookmarks.addService("Meneame","http://meneame.net/submit.php?url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("MisterWong","http://www.mister-wong.com/addurl/?bm_url=__URL__&bm_description=__TITLE__&plugin=soc");
iBeginShare.plugins.builtin.bookmarks.addService("MSNReporter","http://reporter.nl.msn.com/?fn=contribute&Title=__TITLE__&URL=__URL__&cat_id=6&tag_id=31&Remark=");
iBeginShare.plugins.builtin.bookmarks.addService("muti","http://www.muti.co.za/submit?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("MyShare","http://myshare.url.com.tw/index.php?func=newurl&url=__URL__&desc=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("N4G","http://www.n4g.com/tips.aspx?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Netvouz","http://www.netvouz.com/action/submitBookmark?url=__URL__&title=__TITLE__&popup=no");
iBeginShare.plugins.builtin.bookmarks.addService("NewsVine","http://www.newsvine.com/_tools/seed&save?u=__URL__&h=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("NuJIJ","http://nujij.nl/jij.lynkx?t=__TITLE__&u=__URL__&b=");
iBeginShare.plugins.builtin.bookmarks.addService("Oknotizie","http://oknotizie.virgilio.it/post?title=__TITLE__&url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("PingFm","http://ping.fm/ref/?link=__URL__&title=__TITLE__&body=");
iBeginShare.plugins.builtin.bookmarks.addService("Posterous","http://posterous.com/share?linkto=__URL__&title=__TITLE__&selection=");
iBeginShare.plugins.builtin.bookmarks.addService("PrintFriendly","http://www.printfriendly.com/printc?url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Propeller","http://www.propeller.com/submit/?url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Pusha","http://www.pusha.se/posta?url=__URL__&title=__TITLE__&description=");
iBeginShare.plugins.builtin.bookmarks.addService("Ratimarks","http://ratimarks.org/bookmarks.php/?action=add&address=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Rec6","http://rec6.via6.com/link.php?url=__URL__&=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Scoopeo","http://www.scoopeo.com/scoop/new?newurl=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Segnalo","http://segnalo.alice.it/post.html.php?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Simpy","http://www.simpy.com/simpy/LinkAdd.do?href=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Slashdot","http://slashdot.org/bookmark.pl?title=__TITLE__&url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Socialogs","http://socialogs.com/add_story.php?story_url=__URL__&story_title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Sonico","http://www.sonico.com/share.php?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Sphinn","http://sphinn.com/index.php?c=post&m=submit&link=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("StudiVZ","http://www.studivz.net/Link/Selection/Url/?u=__URL__&desc=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Symbaloo","http://www.symbaloo.com/nl/add/url=__URL__&title=__TITLE__&icon=http%3A//static01.symbaloo.com/_img/favicon.png");
iBeginShare.plugins.builtin.bookmarks.addService("ThisNext","http://www.thisnext.com/pick/new/submit/sociable/?url=__URL__&name=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Tipd","http://tipd.com/submit.php?url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Translate","http://translate.google.com/translate?&u=__URL__&sl=&tl=");
iBeginShare.plugins.builtin.bookmarks.addService("Tumblr","http://www.tumblr.com/share?v=3&u=__URL__&t=__TITLE__&s=");
iBeginShare.plugins.builtin.bookmarks.addService("Upnews","http://www.upnews.it/submit?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Webnews","http://www.webnews.de/einstellen?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Webride","http://webride.org/discuss/split.php?uri=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Wists","http://wists.com/r.php?r=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Wykop","http://www.wykop.pl/dodaj?url=__URL__");
iBeginShare.plugins.builtin.bookmarks.addService("Xerpi","http://www.xerpi.com/block/add_link_from_extension?url=__URL__&title=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("Yigg","http://yigg.de/neu?exturl=__URL__&exttitle=__TITLE__");
iBeginShare.plugins.builtin.bookmarks.addService("","");
iBeginShare.plugins.builtin.bookmarks.addService("","");
iBeginShare.plugins.builtin.bookmarks.addService("","");
iBeginShare.plugins.builtin.bookmarks.addService("","");
iBeginShare.plugins.register(iBeginShare.plugins.builtin.bookmarks)
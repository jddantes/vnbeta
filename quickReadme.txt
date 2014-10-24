Quick Start Guide / The Trashtalk-Rant Guide to C :))

strMap (strMap.h)

-sariling implementation ng map
-parang array, kaso imbis na arr[number], map[string]
-so for example meron kang map na laman etong key-value pairs:
	{food:pizza, ice:cold, sleep:zzz}
	tas sabihin nating name ng map ay map
	and gusto mo yung pizza kunwari
	you go map[food]
-now noob c so walang ganyan (c++ cool so meron; mas cool python kasi halo-halo)
-but yun nga, gumawa ako dun sa strMap.h, so kunwari yung map[food],
	you access it by calling mapVal(&map, "food")
	convenient di ba xD


postData, more on maps

-so ayun, cool ang maps
-now useful to lalo na dun sa pagprocess nung string na pinapasa sa post
-so dun sa story.c, merong fgets(buffer etc), tapos the next line makePostData
-yun na yun, one line xD
-so ginagawa nun kinukuha yung raw string, tas prinoprocess at nilalagay sa postData
	yung key-value pairs
-so kunwari kung yung buffer mo ay: state=scenechuchu&food=nice&manga=cool
	tas tinawag mo yung makePostData(&postData, buffer)
	mangyayari, malalagay sa postData yung {state:scenechuchu, food:nice, manga:cool}
	tas kung gusto mo iaccess yung say cool, ganun lang ulit
	mapVal(&postData, "manga")
-matino din yung return values nung map functions (lahat nasa strMap, patingnan na lang)
	so like yung sa mapVal, string siya, so pwede na gamitin agad (char * kasi binabalik)
	e.g. strcpy(dest, mapVal(&postData, "manga"))
		printf("%s\n", mapVal(&postData, "manga"))

-meron din palang nagrereturn nung strPair * (tawag ko itr for iterator), na it points to the key	-value pair mismo
	useful to kung gusto mo kunin yung key
	e.g strPair * itr = mapFind(&postData, "state")
	so from there pwede mo maacess yung itr->key ("state") at itr->val ("scenechuchu"), and again
	pwede na gamitin (printf strcpy etc)


strjoin, strapp, editing map values

-another reason why cool c++ ay pwede mag add ng strings, variable man or literal
	e.g. char a[] = "asdf"; char b[] = "woot"
	tas say may string kang c,
	pwedeng c = a + " extra literal anywhere " + b + " as in kahit saan"
-and pag sa c, you do
	strcat(c, a);
	strcat(c," extra literal anywhere ");
	strcat(c, b);
	strcat(c, " as in kahit saan");
-now annoying yun, so yay strjoin. Konting google and you have stuff for variable number of arguments xD
-so to do the above stuff, you do strjoin(c, a, " extra literal anywhere ", b, " as in kahit saan", NULL); xD
-NOTE NA KAILANGAN YUNG NULL SA DULO, var. arg. stuff (va for short)
-rinereturn nya yung string kung san mo nilagay (first argument), so again pwede gamitin agad,
	e.g. strcpy(dest, strjoin(src, "chuchu", anotherstr, "etc", NULL));
-note na kailangan may array kung san mo idudump yung whole thing, kasi di sya nagrereturn ng string in itself.
	i.e. di pwedeng strcpy(dest, strjoin("asdf", anotherstr, "etc", NULL))
	DAPAT MAY PAGLAGYAN
	to remedy this, pwede mag put aside ng dummy array, in this case nullArr, nasa taas ng story.c
	=> so pwede na na strcpy(dest, strjoin(nullArr, "asdf", anotherstr, "etc", NULL))

-so ayun, strapp kung append lang
-Dun sa maps, may mapUpdate kung gusto mo iset yung value ng key
	e.g. from food:pizza to food:cake
	sa c++ map[food] = cake lang
	satin we do mapUpdate(&map, "food", "cake");
-kung gusto mo iappend, mapApp gamit
	e.g. from food:pizza to food:pizza and cake
	sa c++ map[food] += " and cake"
	satin we do mapApp(&map, "food", " and cake")


readInput (inputReader.h)
-dun sa html files pala, minsan may makikita kang {% keyword %}
-convenient yan, kasi from the cgi thingo (which we can call the controller/logic), irereplace nya yung {% keyword %} with something appropriate
-for example, sa story.html may divs na allocated for speech/speaker
-so from the controller (story.c), dun sa render html part, call lang sya sa readInput(template fp, output stream, map na may values)
-so kunwari sa html meron kang <div> {%speaker%} </div>, tas dun sa detailsMap mo na process mo na meron kang kv (key-value) pair na speaker:Michael
	sa html marerender ay <div> Michael </div>
-pag di nahanap yung keyword tatanggalin lang siya; useful to later pag mageeliminate kung ano yung ididisplay based kung may value o wala (i.e. between choices ba or dialogue yung ididisplay)


----

Implementation stuff

Scenes
-So files. Nasa scenes folder yun. .vn for dialogues, .ch for parts na pipili/may choices, .chp for yung pupuntahan after the choices (originally .cha kaso meron pala nun)
-So format ay header, -----, etc. Sa headers pala later ilalagay yung scene stuff, like background image etc
-tas may tags like [tag] sa start ng line, tas may corresponding[/tag].
-take note na dapat nasa simula tong tags na to, unlike yung {% %} kanina na kahit saan. Pwede mag lagay ng extra newlines/blank lines though.

-d for dialogue, c for choices, b for branch as in where to go after
-tas ganun din, per line din yung dialogues. separated by ':', in the format speaker:speech
-pag c naman, in the format whereToGo:speech

State
-Yung sa state pala, originally scene,dialogue lang. so per scene maraming dialogue lines, so pag di pa naubos umiikot lang yung
cgi to itself, tas iniincrement lang yung d_index to display the next line, until time to branch na.
-Well for saving naisip ko kailangan din pala nung usr (1-10). Practially yun yung slot_id sa mga saved slots.
-So ayun, yung readTriple saka makeTriple for this purpose yun, nasa vnutil.h, para auto-parse na yung laman nung things :))

---

Todos

Save
-Ayusin pa yung sa save. Right now nag sasave naman, kaso after saving nagproproceed sya sa next na scene/dialogue na. Nothing much wrong with that, kasi dun ka rin naman talaga pupunta afterward, pero mas cool kung bumabalik sa current scene, as if di nya apekto yung gameplay

Store
-Ayun store. Pero save muna dapat polido :))

Image++, itsura syempre etc

SANA MAG EXTEND ULET HAHA

P. S.
Ayun. So sa mga scenes dyan di ko pa nilalagay yung ginawa mo. As you can see madali na (ish) lang sya ilipat pag buo, kasi may format na yung thing. Naisip ko kasi na naka name:speech din yung nasa word so ayun :))

P. P. S.
PETITION TAYO MAGPAEXTEND :))





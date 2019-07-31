#include <assert.h>

#include<student/cpu.h>
#include<student/gpu.h>
#include<stack/stack.h>
#include<student/window.h>
#include<student/application.h>
#include<student/parseArguments.h>
#include<tests/conformanceTests.h>
#include<tests/performanceTest.h>
#include<tests/takeScreenShot.h>

int main(int argc,char*argv[]){
  char const* groundTruthFile = "../tests/output.bmp";
  int32_t     windowWidth     = 500;
  int32_t     windowHeight    = 500;
  char const* applicationName = "izgProject";

  Arguments args;
  arguments_parse(&args,argc,argv);

  if(args.runConformanceTests){
    runConformanceTests();
    return 0;
  }

  if(args.runPerformanceTests){
    runPerformanceTest();
    return 0;
  }

  if(args.takeScreenShot){
    takeScreenShot();
    return 0;
  }

  AppData app;

  app_init(&app,windowWidth,windowHeight,applicationName);
  app_start(&app);
  app_free(&app);

  return 0;
}

/**
 * \mainpage Izg project.
 *
 * \section zadani Zadání projektu do předmětu IZG.
 *
 * \tableofcontents
 *
 * \image html images/bunny.png "Výstup projektu"
 *
 * Vašim úkolem je naimplementovat softwarový vykreslovací řetězec (pipeline).
 * Pomocí vykreslovacího řetězce vizualizovat model králička s phongovým
 * osvělovacím modelem a phongovým stínováním.
 * V tomto projektu nebudeme pracovat s GPU, ale budeme se snažit simulovat její
 * práci.
 * Cílem je pochopit jak vykreslovací řetěc funguje, z čeho je složený a jaká
 * data se v něm pohybují.
 *
 * Váš úkol je složen ze tří částí: naprogramovat vykreslovací řetězec,
 * naprogramovat vykreslování králička pomocí cpu_* příkazů a napsat
 * shadery pro phongův osvětlovací model.
 * Musíte doplnit implementace několika funkcí a rozchodit kreslení modelu
 * králička.
 * Funkce mají pevně daný interface (Vstupy a výstupy).
 * Seznam všech úkolů naleznete zde \link todo.html \endlink.
 * Úkoly týkající se pouze GPU strany naleznete zde \link gpu_side GPU\endlink.
 * Úkoly týkající se pouze CPU strany naleznete zde \link cpu_side CPU\endlink.
 * Úkoly týkající se pouze shaderů naleznete zde \link shader_side
 * Shadery\endlink.
 *
 * Každý úkol má přiřazen akceptační test, takže si můžete snadno ověřit
 * funkčnosti vaší implementace.
 *
 * V projektu jsou přítomny i nějaké další příklady.
 * Tyto příklady můžete využít pro inspiraci a návod jak napsat cpu stranu a
 * shadery.
 *
 * Pro implementaci gpu teorii na této stránce,
 * doxygen dokumentaci, video návodu a látce probírané na přednáškách.
 *
 * <a href="https://www.youtube.com/watch?v=Y2jPx8d20fE">Video Návod</a>.
 *
 *
 * \section rozdeleni Rozdělení
 *
 * Projekt je rozdělen do několika podsložek:
 *
 * <b>student/</b> Tato složka obsahuje soubory, které využijete při
 * implementaci projektu.
 * Složka obsahuje soubory, které budete odevzávat a podpůrné knihovny.
 * Všechny soubory v této složce jsou napsány v jazyce C abyste se mohli podívat
 * jak jednotlivé části fungují.
 *
 * <b>tests/</b> Tato složka obsahuje akceptační a performanční testy projektu.
 * Akceptační testy jsou napsány s využitím knihovny catch.
 * Testy jsou rozděleny do testovacích případů (TEST_CASE). Daný TEST_CASE
 * testuje jednu podčást projektu.
 *
 * <b>queue/</b> Tato složka obsahuje implementaci fronty.
 * <b>vector/</b> Tato složka obsahuje implementaci vectoru (c++)
 * <b>stack/</b> Tato složka obsahuje implementaci zásobníku.
 * <b>errorCodes/</b> Tato složka obsahuje chybové kódy.
 *
 * <b>doc/</b> Tato složka obsahuje doxygen dokumentaci projektu.
 * Můžete ji přegenerovat pomocí příkazu doxygen spuštěného v root adresáři
 * projektu.
 *
 * <b>3rdParty/</b> Tato složka obsahuje hlavičkový soubor pro unit testy -
 * catch.hpp.
 * Z pohledu projektu je nezajímavá.
 * Catch je knihovna složená pouze z hlavičkového souboru napsaného v jazyce
 * C++.
 * Poskytuje několik užitečných maker pro svoji obsluhu.
 * TEST_CASE - testovací případ (například pro testování jedné funkce).
 * WHEN - toto makro popisuje způsob použití (například volání funkce s
 * parametery nastavenými na krajní hodnoty).
 * REQUIRE - toto makro vyhodnotí podmínku a případně vypíše chybu (například
 * chcete ověřit, že vaše funkce vrátila správnou hodnotu).
 *
 * <b>images/</b> Tato složka obsahuje doprovodné obrázky pro dokumentaci v
 * doxygenu.
 * Z pohledu projektu je nezajímavá.
 *
 * Složka student/ obsahuje soubory, které se vás přímo týkají:
 *
 * <b>drawTriangles.c</b> obsahuje funkci pro vykreslování trojúhelníků - tu máte naprogramovat
 * editovat.
 *
 * <b>phongMethod.c</b> obsahuje cpu stranu a shadery pro vykresleni králička - toto máte naprogramovat.
 * editovat.
 *
 * Funkce s předponou cpu_ můžou být volány pouze na straně, v souboru
 * phongMethod.c.
 * Funkce s předponou gpu_ můžou být volány pouze na straně GPU, v souboru
 * drawTriangles.c.
 * Funkce bez předpony můžou být volány na CPU, GPU tak v rámci shaderu.
 *
 * Struktury, které se vyskytují pouze na GPU straně jsou uvozeny prefixem GPU.
 * Struktury bez předpony lze využít jak na CPU tak GPU straně či v shaderu.
 *
 * Projekt je postaven nad filozofií OpenGL. Spousta funkcí má podobné jméno.
 *
 * \section teorie Teorie
 *
 * Typické grafické API (OpenGL/Vulkan/DirectX) je složeno ze 2 částí: CPU a GPU
 * strany.
 *
 * CPU strana se obvykle stará o tyto úkoly:
 *  - Příprava dat pro kreslení (modely, textury, matice, ...)
 *  - Upload dat na GPU a nastavení GPU
 *  - Spuštění vykreslení
 *
 * GPU strana je složena ze dvou částí: video paměti a zobrazovacího řetězce.
 * Vykreslovací řetězec se skládá z několika částí:
 * - Vertex Puller (načtení dat z bufferů a výpočet gl_VertexID)
 * - Vertex Processor (vyvolávání vertex shaderu)
 * - Primitive Assembly (sestavení primitiva/trojúhelníku)
 * - Clipping (nebudete muset implementovat)
 * - Perspektivní dělení
 * - View-port transformace
 * - Rasterize (středy pixelů musí ležet uvnitř trojúhelníku, perspektivně korektní interpolace atributů)
 * - Fragment Processor (vyvolávání fragment shaderu)
 * - Per-Fragment Operations (depth test) <br>
 *
 * <b>Vertex Puller</b> Vertex puller je složen z N čtecích hlav, které sestavují vrchol.
 * Jeho úkolem je spočítat číslo vrcholů gl_VertexID.
 * Číslo vrcholu je dáno pořadovým číslem vyvolání (invokací) vertex shaderu - to v případě že není použito indexování.
 * Jinak je číslo dáno indexem v indexačnímu bufferu.
 * Indexační buffer může mít růzou velikost indexu - 8bit, 16bit a 32bit.
 * Pokud je zapnuto indexování, pak je  číslo vrcholu dáno položnou v indexačním bufferu, kde 
 * položka (index) v bufferu je vybráno na základě čísla invokace vertex shaderu.
 * Dalším úkolem vertex pulleru je připravit aributy vrcholu, který vstupuje do vertex shaderu.
 * GPUInVertex je složen z M atributů, každý odpovídá jedné čtecí hlavě z vertex pulleru.
 * Čtecí hlava GPUVertexPullerHead obsahuje nastavení - offset, stride, size a buffer id.
 * Pokud je čtecí hlava povolena, měla by zkopírovat data (o velikosti size) z bufferu od daného offsetu, s krokem stride.
 * Všechny velikosti jsou v bajtech.
 * Krok se použije při čtení různých vrcholů: atributy by měly být čteny z adresy: buf_ptr + offset + stride*gl_VertexID
 * <br>
 * <b>Vertex Processor</b>
 * Vertex processor vyvolává vertex shader.
 * Vertex shader by měl obržet správna data ve struktuře GPUVertexShaderData, která je složena ze vstupního vrcholu, výstupního vrcholu
 * a uniformních proměnných GPUUniforms.
 * Uniformní proměnné jsou přiřazeny ke každému programu zvlášť a jsou uloženy ve struktuře GPUProgram.
 * Uniformní proměnné zůstávání konstantní v průběhu vykresovácího příkazu.
 * Vertex shader by měl zapisovat do výstupního vrcholu GPUOutVertex - do atributů a do proměnné gl_Position.
 * <br>
 * <b>Primitive Assembly</b>
 * Sestavení primitiv by mělo dát dohromady primitiva (trojúhelníky) z N posobě jdoucích vrcholů.
 * <br>
 * <b>Perspektivní dělení</b>
 * Perspektivní dělení následuje za clippingem/cullingem (není součást projektu) a provádí převod z homogenních souřadnic na kartézské pomocí dělení w.
 * <br>
 * <b>View-port transformace</b>
 * View-port transformace převádí xy rozsah z intervalu <-1,+1> na rozsah <0,velikost framebufferu).
 * <br>
 * <b>Rasterizace</b>
 * Rasterizace rasterizuje transformovaný trojúhelník.
 * Rasterizace produkuje fragmenty v případě, že <b>střed</b> pixelu leží uvnitř trojúhelníka.
 * Rasterizace by měla zapsat souřadnice fragmentu do proměnné gl_FragCoord.
 * Pozice fragmentu obsahuje v x,y souřadnici na obrazovce a v z hloubku.
 * Další úkol rasterizace je interpolace vertex attributů do fragment attributů.
 * Atributy které jsou posílány z vertex shaderu do fragment shaderu jsou poznačeny v proměnné vs2fsType ve struktuře GPUProgram.
 * Úkolem rasterizéru je perspektivně korektně interpolovat atributy.
 * Perspektivně korektní interpolace využívá pro interpolaci barycentrické koordináty spočítané z 3D trojúhelníku a ne z 2D trojúhelníku.
 * Interpolaci je možné provést pomocí: (att0*l0/h0 + att1*l1/h1 + att2*l2/h2) / (l0/h0 + l1/h1 + l2/h2).
 * Kde l0,l1,l2 jsou barycentrické koordináty ve 2D, h0,h1,h2 je homogenní složka vrcholů a att0,att1, att2 je atribut vrcholu.
 * <br>
 * <b>Fragment processor</b>
 * Fragment processor spouští fragment shader nad každým fragmentem.
 * Data pro fragment shader jsou uložena ve struktuře GPUFragmentShaderData.
 * Struktura je složena ze tří položek: GPUUniforms, GPUInFragment a GPUOutFragment.
 * Fragment processor by měl správně vyplnit struktury GPUInFragment a GPUUniforms z výsledků rasterizace.
 * <br>
 * <b>Per-fragment operace</b>
 * Per-fragment operace provádí depth test. Ověření zda je nový fragment blíže než hloubka poznačená ve framebufferu.
 * Pokud je hloubka nového fragment menší, barva a hloubka fragmentu je zapsána do framebufferu.
 * Dejte pozor na přetečení rozsahu gl_FragColor. Před zápisem je nutné ořezat barvu do rozsahu <0,1>.
 * <br>
 *
 * \subsection terminologie Terminologie
 *
 * \b Vertex je kolekce několika vertex atributů.
 * Tyto atributy mají svůj typ a počet komponent.
 * Každý vertex atribut má nějaký význam (pozice, hmotnost, texturovací
 * koordináty), které mu přiřadí programátor/modelátor.
 * Z několika vrcholů je složeno primitivum (trojúhelník, úsečka, ...)
 *
 * <b>Vertex atribut</b> je jedna vlastnost vrcholu (pozice, normála,
 * texturovací koordináty, hmotnost, ...).
 * Atribut je složen z 1,2,3 nebo 4 komponent daného typu (FLOAT, INT, ...).
 * Sémantika atributu není pevně stanovena (atributy mají pouze pořadové číslo -
 * attribIndex) a je na každém programátorovi/modelátorovi, jakou sémantiku
 * atributu přidělí.
 * \image html images/primitive.svg "Vertex attributes"
 *
 * \b Fragment je kolekce několika atributů (podobně jako Vertex).
 * Tyto atributy mají svůj typ a počet komponent.
 * Fragmenty jsou produkovány resterizací, kde jsou atributy fragmetů vypočítány
 * z vertex atributů pomocí interpolace.
 * Fragment si lze představit jako útržek původního primitiva.
 *
 * <b>Fragment atribut</b> je jedna vlastnost fragmentu (podobně jako vertex
 * atribut).
 *
 * <b>Interpolace</b> Při přechodu mezi vertex atributem a fragment atributem
 * dochází k interpolaci atributů.
 * Atributy jsou váhovány podle pozice fragmentu v trojúhelníku (barycentrické
 * koordináty).
 * \image html images/interpolation.svg "Vertex attribute interpolation"
 *
 * <b>Vertex Processor</b> (často označován za Vertex Shader) je funkční blok,
 * který je vykonáván nad každým vertexem.
 * Jeho vstup i výstup je Vertex. Výstupní vertex má obvykle jiné vertex
 * atributy než vstupní vertex.
 * Výstupní vertex má vždy atribut - gl_Position (pozice vertexu v clip-space).
 * Vstupní vertex má vždy atribut - gl_VertexID (číslo vrcholu, s ohledem na indexování).
 * Vertex Processor se obvykle stará o transformace vrcholů modelu (posuny,
 * rotace, projekce).
 * Jelikož Vertex Processor pracuje po vrcholech, je vhodný pro efekty jako
 * vlnění na vodní hladině, displacement mapping apod.
 * Vertex Processor má informace pouze o jednom vrcholu v daném čase (neví nic o
 * sousednostech vrcholů).
 * Vertex processor je programovatelný.
 *
 * <b>Fragment Processor</b> (často označován za Fragment Shader/Pixel Shader)
 * je funkční blok, který je vykonáván nad každým fragmentem.
 * Jeho vstup i výstup je Fragment. Výstupní fragment má obykle jiné attributy.
 * Fragment processor je programovatelný.
 *
 * <b>Shader</b> je program/funkce, který běží na některé z programovatelných
 * částí zobrazovacího řetezce.
 * Shader má vstupy a výstupy, které se mění s každou jeho invokací.
 * Shader má také vstupy, které zůstávají konstantní a nejsou závislé na číslu
 * invokace shaderu.
 * Shaderů je několik typů, v tomto projektu se používají pouze 2 - vertex
 * shader a fragment shader.
 * V tomto projektu jsou shadery reprezentovány pomocí standardních Cčkovských
 * funkcí.
 *
 * <b>Vertex Shader</b> je program, který běží na vertex processoru.
 * Jeho vstupní interface obsahuje: vertex, uniformní proměnné a další proměnné
 * (číslo vrcholu gl_VertexID, ...).
 * Jeho výstupní inteface je vertex, který vždy obsahuje proměnnou gl_Position -
 * pozici vertexu v clip-space.
 *
 * <b>Fragment Shader</b> je program, který běží na fragment processoru.
 * Jeho vstupní interface obsahuje: fragment, uniformní proměnné a proměnné
 * (souřadnici fragmentu ve screen-space gl_FragCoord, ...).
 * gl_FragCoord.xy - souřadnice ve screen space
 * gl_FragCoord.z - hloubka
 * Jeho výstupní interface je fragment.
 * V projektu obsahuje atribut gl_FragColor - pro výstupní barvu.
 *
 * <b>Shader Program</b> je kolekce programů, které běží na programovatelných
 * částech zobrazovacího řetězce.
 * Obsahuje vždy maximálně jeden shader daného typu.
 * V tompto projektu je program reprezentován pomocí dvou ukazatelů na funkce.
 * \image html images/shader_program.svg "This shader program is composed of vertex shader and fragment shader"
 *
 * <b>Buffer</b> je lineární pole dat ve video paměti na GPU.
 * Do bufferů se ukládají vertex attributy vextexů modelů nebo indexy na vrcholy
 * pro indexované vykreslování.
 *
 * <b>Uniformní proměnná</b> je proměná uložená v konstantní paměti GPU. Všechny
 * programovatelné bloky zobrazovacího řetězce z nich mohou pouze číst.
 * Jejich hodnota zůstává stejná v průběhu kreslení (nemění se v závislosti na
 * číslu vertexu nebo fragmentu). Jejich hodnodu lze změnit z CPU strany pomocí
 * funkcí jako je
 * uniform1f, uniform1i, uniform2f, uniformMatrix4fv apod. Uniformní proměnné
 * jsou vhodné například pro uložení transformačních matic nebo uložení času.
 *
 * <b>Vertex Puller</b> se stará o přípravů vrcholů.
 * K tomuto účelu má tabulku s nastavením.
 * Vertex puller si můžete představit jako sadu čtecích hlav.
 * Každá čtecí hlava se stará o přípravu jednoho vertex atributu.
 * Mezi nastavení čtecí hlavy patří: ukazatel na začátek bufferu, offset a krok.
 * Vertex puller může obsahovat indexování.
 *
 * <b>Zobrazovací řetězec</b> je obvykle kus hardware na grafické kartě, který
 * se stará o vyreslování.
 * Grafická karta je složena ze dvou částí: paměti a zobrazovacího řetězce.
 * V paměti se nacházejí buffery, textury, uniformní proměnné, programy,
 * nastavení vertex pulleru a framebuffery.
 * Pokud se spustí kreslení N vrcholů, je vertex puller spuštěn N krát a sestaví
 * N vrcholů.
 * Nad každým vrcholem je puštěn vertex shader.
 * Výstupem vertex shaderu je nový vrchol.
 * Blok sestavení primitiv "si počká" na 3 vrcholy z vertex shaderu (pro
 * trojúhelník) a vloží je do jedné struktury.
 * Blok clipping ořeže trojúhelníky pohledovým jehlanem.
 * Následuje perspektivní dělení, které vydělí pozice vertexů homogenní složkou.
 * Poté následuje viewport transformace, která podělené vrcholy transformuje do
 * rozlišení obrazovky.
 * Rasterizace trojúhelníky nařeže na fragmenty a interpoluje vertex atributy.
 * Nad každým fragmentem je spuštěn fragment shader.
 * Než jsou fragmenty zapsány zpět do paměti GPU (framebufferu) jsou provedeny
 * per-fragment operace (zjištění viditelnosti fragmentů podle hloubky uchované
 * v depth bufferu).
 * \image html images/rendering_pipeline.svg "Simplified rendering pipeline"
 *
 * <b>ObjectID</b> je číslo odkazující se na jeden konkrétní objekt na GPU.
 * Každy buffer, program, vertex puller má přiřazeno/rezervováno takové číslo
 * (BufferID, ProgramID, VertexPullerID).
 *
 * <b>Uniformní lokace</b> je číslo, které reprezentuje jednu uniformní
 * proměnnou.
 *
 * <b>VertexShaderInvocation</b> je pořadové číslo vyvolání vertex shaderu.
 *
 * <b>gl_VertexID</b> je číslo vrcholu, které je vypočítáno pomocí indexování a
 * pořadového čísla vyvolání vertex shaderu.
 *
 * <b>Indexované kreslení</b> je způsob snížení redundance dat s využitím indexů
 * na vrcholy.
 * \image html images/drawElements.svg "Difference between indexed and non-indexed drawing"
 *
 * \section sestaveni Sestavení
 *
 * Projekt byl testován na Ubuntu 18.04, Visual Studio 2015.
 * Projekt vyžaduje 64 bitové sestavení.
 * Projekt využívá build systém <a href="https://cmake.org/">CMAKE</a>.
 * CMake je program, který na základně konfiguračních souborů "CMakeLists.txt"
 * vytvoří "makefile" v daném vývojovém prostředí.
 * Dokáže generovat makefile pro Linux, mingw, solution file pro Microsoft
 * Visual Studio, a další.
 * Postup:
 * -# Zkompilovat a nainstalovat SDL2 pomocí CMAKE - toto vyprodukuje adresářovou strukturu installSložkaSDL2/
 *  V té strutkuře jsou knihovny (*.dll, *.lib, *.so, *.a), inkludy (*.h) a cmake skripty (*.cmake).
 * -# stáhnout projekt
 * -# rozbalit projekt
 * -# ve složce build spusťte "cmake-gui .." případně "ccmake .."
 * -# vyberte si překladovou platformu (64 bit).
 * -# configure
 * -# nastavte do proměnné SDL2_DIR cestu k SDLConfig.cmake souboru (obvykle: installSložkaSDL2/lib/cmake/SDL2/
 * -# generate
 * -# make nebo otevřete vygenerovnou Microsoft Visual Studio Solution soubor.
 * -# pokud vám projekt nejde spustit (chybí SDL2.dll) překupírujte dané DLL k exači projetku.
 *
 * Projekt vyžaduje pro sestavení knihovnu <a
 * href="https://www.libsdl.org/download-2.0.php">SDL2</a>.
 * <b>Musíte</b> si knihovnu stáhnou, zkompilovat a nainstalovat pomocí CMAKE.
 * Projekt využívat targety SDL2::SDL2 a SDL2::SDL2main, které jsou uvedeny v cmake configurácích pokud je knihovna správně zkompilována a nainstalována.
 *
 * \section spousteni Spouštění
 *
 * Projekt je možné po úspěšném přeložení pustit přes aplikaci
 * <b>izgProject</b>.
 * Projekt akceptuje několik argumentů příkazové řádky:
 * - <b>-c ../tests/output.bmp</b> spustí akceptační testy, soubor odkazuje na
 * obrázek s očekávaným výstupem.
 * - <b>-p</b> spustí performanční test.
 *
 * \section ovladani Ovládání
 * Program se ovládá pomocí myši a klávesnice:
 * - stisknuté levé tlačítko myši + pohyb myší - rotace kamery
 * - stisknuté pravé tlačítko myši + pohyb myší - přiblížení kamery
 * - "n" - přepne na další scénu/metodu
 *   "p" - přepne na předcházející scénu/metodu
 *
 * \section odevzdavani Odevzdávání
 *
 * Před odevzdáváním si zkontrolujte, že váš projekt lze přeložit na merlinovi.
 * Zkopirujte projekt na merlin a spusťte skript: <b>./merlinCompilationTest.sh</b>.
 * Odevzdávejte pouze soubory drawTriangles.c a
 * phongMethod.c. Soubory zabalte do archivu proj.zip. Po rozbalení archivu
 * se <b>NESMÍ</b> vytvořit žádná složka. Příkazy pro ověření na Linuxu: zip
 * proj.zip drawTriangles.c phongMethod.c, unzip proj.zip.
 * Studenti pracují na řešení projektu samostatně a každý odevzdá své vlastní
 * řešení.
 * Poraďte si, ale řešení vypracujte samostatně!
 *
 * \section hodnoceni Hodnocení
 *
 * Množství bodů, které dostanete, je odvozeno od množství splněných
 * akceptačních testů a podle toho, zda vám to kreslí správně (s jistou
 * tolerancí kvůli nepřesnosti floatové aritmetiky).
 * Automatické opravování má k dispozici větší množství akceptačních testů
 * (kdyby někoho napadlo je obejít).
 * Pokud vám aplikace spadne v rámci testů, dostanete 0 bodů.
 * Pokud aplikace nepůjde přeložit, dostanete 0 bodů.
 *
 * \section soutez Soutěž
 *
 * Pokud váš projekt obdrží plný počet bodů, bude zařazen do soutěže o
 * nejrychlejší implementaci zobrazovacího řetězce.
 * Můžete přeimplementovat cokoliv v odevzdávaných souborech pokud to projde
 * akceptačními testy a kompilací.
 *
 * \section zaver Závěrem
 * Ať se dílo daří a ať vás grafika alespoň trochu baví!
 * V případě potřeby se nebojte zeptat (na fóru nebo napište přímo vedoucímu
 * projektu imilet@fit.vutbr.cz).
 */

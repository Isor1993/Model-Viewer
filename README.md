# Model Viewer

C++/OpenGL-3D-Model-Viewer — die Aufgabe „3D Model-Viewer" aus Modul
5FSC0XD101 „Creative Studio 1" (Games Programming). Zeigt drei
prozedural erzeugte, texturierte 3D-Modelle (Kugel, Würfel, Pyramide)
mit frei drehbarer Orbit-Kamera, einem Richtungslicht, einer
Cubemap-Skybox und einem Stylized-Toon-Look mit Inverted-Hull-Outline.

## Bauen und Starten

- `ModelViewer.sln` in Visual Studio 2026 öffnen, x64 wählen, F5.
- Alle externen Abhängigkeiten liegen mit im Repo unter `Libraries/` —
  kein Paketmanager, kein Zusatz-Setup:
  - GLFW 3.4 (Fenster und Eingabe, vorkompiliert)
  - GLAD (OpenGL-4.6-Core-Funktionslader, generierter Quellcode)
  - GLM (Mathematik, header-only)
  - stb_image (Bildladen, Single-Header)
- Assets (`Shaders/`, `Textures/`) werden relativ zum Projektordner
  geladen — die Exe daher über Visual Studio starten.

## Steuerung

- Linke Maustaste ziehen — Kamera um das Modell drehen
- Mausrad — Zoom (mit Anschlägen)
- Esc — Beenden

## Pflichtpunkte — und wo sie erfüllt sind

| Anforderung | Wo |
|---|---|
| Anwendungsfenster | `Window.*` — GLFW, OpenGL 4.6 Core, 4x MSAA |
| Programmierbare Render-Pipeline (Vertex- und Pixel-Shader) | `Shaders/` — drei selbstgeschriebene Paare: toon, unlit, skybox |
| Sinnvoll strukturierter, erweiterbarer Programmaufbau | `Application.*` besitzt die Szene (Zeiger-Member, gruppierte Initialisierung, je Objekt eine Render-Station); jede RAII-Klasse besitzt genau eine GPU-Ressource; Projektordner nach Typ getrennt (Source, Header, Shaders, Textures) |
| Mindestens ein texturiertes 3D-Modell | drei prozedural erzeugte Modelle: UV-Kugel, Würfel und Pyramide (`Geometry.cpp`) |
| Mindestens ein Licht | Richtungslicht als Uniforms, sichtbar gemacht durch die Sonnenscheibe |
| Bewegbare Kamera | Orbit-Kamera (`Camera.*`) — Drehen, Neigen und Zoom mit Anschlägen |
| Skybox oder Ähnliches | echte Cubemap (`SkyBox.*`, CC0-Himmelspaket) |

## Über die Pflicht hinaus

- Stylized-Toon-Shading: weiche smoothstep-Kante um die
  Schattenschwelle, kühler Schatten-Tint statt Schwarz
- Zwei Zusatz-Modelle neben der Pflicht-Kugel: Würfel (harte Kanten
  über eine Normale je Fläche) und Pyramide (Seiten-Normalen per
  Kreuzprodukt), beide mit eigenem Generator und eigener
  CC0-Textur (Holzplanken und Sandstein, ambientcg.com)
- Kontakt-Schatten als plattgedrücktes Objekt-Mesh — jedes Modell
  zeichnet seine eigene Standfläche auf den Boden
- Inverted-Hull-Outline: jedes Modell ein zweites Mal gezeichnet,
  skaliert, schwarz, Vorderseiten per Culling entfernt
- Wiese mit Horizont-Fade, Sonnenscheibe und Kontakt-Schatten
- Warnungsfrei auf Warnstufe 4 inklusive VS-Code-Analyse
  (Fremdbibliotheken als extern ausgenommen)

## Ordnerübersicht

```
ModelViewer.sln
ModelViewer/
  Source/             alle .cpp: main (Einstieg), Application (Szene
                      und Render-Schleife), Geometry (CPU-Mesh-Builder:
                      Kugel, Würfel, Pyramide, Boden, Himmelswürfel),
                      Window, Camera, Shader, Mesh, Texture, SkyBox
  Header/             die zugehörigen Header, eine Klasse je Datei
  Shaders/            toon | unlit | skybox (.vert / .frag)
  Textures/           Ball, Gras, Holz, Sandstein, Skybox/ (Cubemap)
Libraries/            GLFW, GLAD, GLM, stb_image (mitgeliefert)
```

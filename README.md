# Model Viewer

C++/OpenGL-3D-Model-Viewer — die Aufgabe „3D Model-Viewer" aus Modul
5FSC0XD101 „Creative Studio 1" (Games Programming). Zeigt ein
texturiertes 3D-Modell mit frei drehbarer Orbit-Kamera, einem
Richtungslicht, einer Cubemap-Skybox und einem Stylized-Toon-Look mit
Inverted-Hull-Outline.

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
| Sinnvoll strukturierter, erweiterbarer Programmaufbau | `Application.*` besitzt die Szene (Zeiger-Member, gruppierte Initialisierung, je Objekt eine Render-Station); jede RAII-Klasse besitzt genau eine GPU-Ressource |
| Mindestens ein texturiertes 3D-Modell | prozedural erzeugte UV-Kugel (`Geometry.cpp`) mit `Textures/ball_color.png` |
| Mindestens ein Licht | Richtungslicht als Uniforms, sichtbar gemacht durch die Sonnenscheibe |
| Bewegbare Kamera | Orbit-Kamera (`Camera.*`) — Drehen, Neigen und Zoom mit Anschlägen |
| Skybox oder Ähnliches | echte Cubemap (`SkyBox.*`, CC0-Himmelspaket) |

## Über die Pflicht hinaus

- Stylized-Toon-Shading: weiche smoothstep-Kante um die
  Schattenschwelle, kühler Schatten-Tint statt Schwarz
- Inverted-Hull-Outline: die Kugel ein zweites Mal gezeichnet,
  skaliert, schwarz, Vorderseiten per Culling entfernt
- Wiese mit Horizont-Fade, Sonnenscheibe und Kontakt-Schatten
- Warnungsfrei auf Warnstufe 4 inklusive VS-Code-Analyse
  (Fremdbibliotheken als extern ausgenommen)

## Ordnerübersicht

```
ModelViewer.sln
ModelViewer/
  main.cpp            Einstieg: Fenster + Application + Run
  Application.*       besitzt die Szene und die Render-Schleife
  Window.*  Camera.*  Shader.*  Mesh.*  Texture.*  SkyBox.*
  Geometry.*          CPU-Mesh-Builder (Kugel, Boden, Himmelswürfel)
  Shaders/            toon | unlit | skybox (.vert / .frag)
  Textures/           Ball, Gras, Skybox/ (Cubemap-Seiten)
Libraries/            GLFW, GLAD, GLM, stb_image (mitgeliefert)
```

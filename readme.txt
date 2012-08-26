
This is OSX, iOS, Windows GLSL starter kit with C++.

game05.xcodeproj : for OSX
game05iOS.xcodeproj : for iOS5.0~
game05.sln : for Windows VisualStudio2010


co_matrix.hpp
OpenGL ES 2.0では削除されている行列関連をEigenを使ってコツコツ実装。

glPushMatrix() → pushMatrix()
glLoadIdentity() → loadIdentity()
glOrth() → orthoMatrix()
glFrustum() → frustumMatrix()

みたいな感じで各種用意。


co_texture.hpp

pngもしくはpvrtcの画像を読み込んでテクスチャ化。

ngs::Texture testure(filepath, mipmap)
みたいな感じで使う。


co_graph.hpp

テクスチャ付き矩形を描画。uv座標はピクセルで指定。
回転拡大縮小、上下左右フリップなどを実装。


co_easyShader.hpp

シェーダーの各種めんどくさいセットアップをほぼ一括して処理するクラス。

EasyShader shader(vsh, fsh);

shader();
// シェーダーを有効化

glEnableVertexAttribArray(shader.attrib("position"));
glUniformMatrix4fv(shader.uniform("projectionMatrix"), 1, GL_FALSE, matrix);
// ↑attributeとuniform変数へは名前でアクセスできる


あとはサンプルnn_app.hppを読んでね。

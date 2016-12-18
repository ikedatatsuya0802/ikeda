xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 13;
 -0.23660;-0.12000;13.19920;,
 0.00000;0.04000;12.65405;,
 -1.08688;0.04000;11.66086;,
 -1.81016;-0.10000;11.57828;,
 0.23654;-0.12000;13.19920;,
 1.81009;-0.10000;11.57827;,
 1.08688;0.04000;11.66085;,
 -1.08688;0.04000;-12.07247;,
 -1.81016;-0.10000;-12.07245;,
 1.08688;0.04000;-12.07247;,
 1.81009;-0.10000;-12.07245;,
 1.08688;-0.10000;-12.85202;,
 -1.08688;-0.10000;-12.85202;;
 
 10;
 4;0,1,2,3;,
 4;1,4,5,6;,
 4;3,2,7,8;,
 4;2,6,9,7;,
 4;6,5,10,9;,
 4;7,9,11,12;,
 3;1,6,2;,
 3;0,4,1;,
 3;8,7,12;,
 3;11,9,10;;
 
 MeshMaterialList {
  1;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;0.700000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "../mo_e5_1_u/shadow2.png";
   }
  }
 }
 MeshNormals {
  13;
  -0.099028;0.964089;0.246425;,
  0.000005;0.984137;0.177413;,
  0.099039;0.964086;0.246434;,
  -0.194575;0.975270;0.104831;,
  -0.097623;0.993833;0.052597;,
  0.097632;0.993832;0.052601;,
  0.194592;0.975265;0.104840;,
  -0.189316;0.978041;-0.087150;,
  -0.095095;0.991541;-0.088341;,
  0.095103;0.991540;-0.088341;,
  0.189332;0.978038;-0.087150;,
  -0.093993;0.979896;-0.175978;,
  0.094001;0.979896;-0.175979;;
  10;
  4;0,1,4,3;,
  4;1,2,6,5;,
  4;3,4,8,7;,
  4;4,5,9,8;,
  4;5,6,10,9;,
  4;8,9,12,11;,
  3;1,5,4;,
  3;0,2,1;,
  3;7,8,11;,
  3;12,9,10;;
 }
 MeshTextureCoords {
  13;
  0.553118;0.021421;,
  0.483529;0.042630;,
  0.803201;0.081270;,
  1.015929;0.084482;,
  0.413960;0.021421;,
  -0.048851;0.084482;,
  0.163858;0.081270;,
  0.803201;0.969673;,
  1.015929;0.969671;,
  0.163858;0.969673;,
  -0.048851;0.969672;,
  0.163858;1.000001;,
  0.803201;1.000001;;
 }
}

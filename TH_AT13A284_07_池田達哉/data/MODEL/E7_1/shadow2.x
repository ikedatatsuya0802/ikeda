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
 12;
 -1.81020;-0.10000;12.07240;,
 -1.08688;0.04000;12.07240;,
 -1.08688;0.04000;-12.07247;,
 -1.81020;-0.10000;-12.07244;,
 1.08688;0.04000;12.07240;,
 1.08688;0.04000;-12.07247;,
 1.81005;-0.10000;12.07240;,
 1.81005;-0.10000;-12.07244;,
 1.08684;-0.10000;-12.85201;,
 -1.08692;-0.10000;-12.85201;,
 -1.08692;-0.10000;12.85201;,
 1.08684;-0.10000;12.85201;;
 
 9;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;2,5,8,9;,
 3;3,2,9;,
 3;8,5,7;,
 4;1,10,11,4;,
 3;0,10,1;,
 3;11,6,4;;
 
 MeshMaterialList {
  1;
  9;
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
    "../mo_e5_1_u/shadow.png";
   }
  }
 }
 MeshNormals {
  12;
  -0.189303;0.978044;0.087140;,
  -0.095089;0.991542;0.088332;,
  0.095108;0.991540;0.088337;,
  0.189341;0.978036;0.087149;,
  -0.189306;0.978043;-0.087147;,
  -0.095091;0.991541;-0.088339;,
  0.095109;0.991539;-0.088344;,
  0.189343;0.978035;-0.087156;,
  -0.093989;0.979897;-0.175976;,
  0.094007;0.979894;-0.175986;,
  -0.093986;0.979900;0.175961;,
  0.094005;0.979897;0.175971;;
  9;
  4;0,1,5,4;,
  4;1,2,6,5;,
  4;2,3,7,6;,
  4;5,6,9,8;,
  3;4,5,8;,
  3;9,6,7;,
  4;1,10,11,2;,
  3;0,10,1;,
  3;11,3,2;;
 }
 MeshTextureCoords {
  12;
  1.015941;0.030330;,
  0.803201;0.030330;,
  0.803201;0.969673;,
  1.015941;0.969671;,
  0.163858;0.030330;,
  0.163858;0.969673;,
  -0.048838;0.030330;,
  -0.048838;0.969672;,
  0.163871;1.000001;,
  0.803213;1.000001;,
  0.803213;-0.000001;,
  0.163871;-0.000001;;
 }
}

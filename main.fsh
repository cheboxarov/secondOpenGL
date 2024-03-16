uniform sampler2D u_texture;
uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;
varying highp vec4 v_position;

void main(void)
{
    vec4 resultColor = vec4(0,0,0,0);
    vec4 eyePostion = vec4(0,0,0,1);
    vec4 diffMatColor = texture2D(u_texture, v_texcoord);
    vec3 eyeVect = normalize(v_position.xyz - eyePostion.xyz);
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition);
    vec3 reflectLight = normalize(reflect(lightVect, v_normal));
    float len = length(v_position.xyz - eyePostion.xyz);
    float specularFactor = 50.0;
    float ambientFactor = 0.1;


    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect));

    resultColor += diffColor;

    vec4 ambientColor = ambientFactor * diffColor;

    resultColor += ambientColor;

    vec4 specularColor = vec4(1,1,1,1) * u_lightPower *
            pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);  ///
            //(1.0 + 0.25 * pow(len, 2.0));

    resultColor += specularColor;
    gl_FragColor = resultColor;
}

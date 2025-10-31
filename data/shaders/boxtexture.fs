varying vec3 v_world_position;
uniform samplerCube u_texture;
uniform vec3 u_camera_pos;
void main()
{
    vec3 dir = normalize(u_camera_pos - v_world_position);
    gl_FragColor = textureCube(u_texture, dir);
}
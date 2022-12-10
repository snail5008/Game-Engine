pub mod engine_bindings;

use crate::engine_bindings::engine::*;

#[allow(dead_code)]
#[no_mangle]
extern "C" fn main() -> i32 {
    
    let mut frames: i32;
    let square: Vec<f32> = vec![
        -1.,  1.,  0.,
        1., -1.,  0.,
        -1., -1.,  0.,
        -1.,  1.,  0.,
        1.,  1.,  0.,
        1., -1.,  0.,
        ];
        let mut win: Window = Window::create().unwrap();
        let mut cam: Camera = Camera::create(deg2rad(45.0), 800.0, 600.0);
        let mut mesh: Mesh = Mesh::create();
        mesh.create_vbo(
            (std::mem::size_of::<f32>() * square.len()) as u32,
            square.as_ptr()
        );
        mesh.create_default_vao();
        mesh.create_default_program();
        mesh.matrices_init();
        
        while win.should_close() == 0 {
            // update the window (glViewport, projection matrix,
            // cursor position, window size, etc.)
            win.update(&mut cam);
            
            // clear the screen with a colour
            Window::clear(50, 50, 50);

            // update the camera
            cam.update();
            
            // get keyboard input
            if win.keypressed('W' as i32) {
                println!("W");
            }
            
            // update the mesh (draw + send matrices to OpenGL)
            mesh.update(6, cam);

            // swap the front & back buffers and poll for events
            win.swapbuffers();

            // print the fps if a second has elapsed
            frames = fps();
            if frames != -1 {
                println!("FPS: {frames}");
        }
    }
    print_openglerror();
    0
}

pub mod engine {
    use std::ffi::c_void;

    #[derive(Clone, Copy)]
    #[repr(C, align(16))]
    pub struct Mesh {
        vao: u32,
        vbo: u32,
        program: u32,

        model_translation: [[f32; 4]; 4],
        model_scale: [[f32; 4]; 4],
        model_rotation: [[f32; 4]; 4]
    }

    #[derive(Clone, Copy)]
    #[repr(C, align(16))]
    pub struct Camera {
        view: [[f32; 4]; 4],
        proj: [[f32; 4]; 4],

        pub position: [f32; 3],
        direction: [f32; 3],
    }

    #[derive(Clone, Copy)]
    #[repr(C, align(16))]
    pub struct Window {
        window: *mut c_void,
        width: i32,
        height: i32,
        width_old: i32,
        height_old: i32,
        mouse_x: f64,
        mouse_y: f64,
    }

    pub mod raw_bindings {
        use crate::engine_bindings::engine::*;

        #[allow(dead_code)]
        extern "C" {
            pub fn program_create(vertex_source: *const u8, fragment_source: *const u8) -> u32;

            pub fn mesh_create_vbo(mesh: *mut Mesh, mesh_size: u32, mesh_verts: *const f32);
            pub fn mesh_create_default_vao(mesh: *mut Mesh);
            pub fn mesh_create_default_program(mesh: *mut Mesh);
            pub fn mesh_draw(mesh: Mesh, vertcount: u32);
            pub fn mesh_matrices_init(mesh: *mut Mesh);
            pub fn mesh_matrices_update(mesh: *mut Mesh, cam: Camera);
            pub fn mesh_update(mesh: *mut Mesh, vertcount: u32, cam: Camera);

            pub fn window_create() -> Window;
            pub fn window_update(win: *mut Window, cam: *mut Camera);
            pub fn window_swapbuffers(window: *const Window);
            pub fn window_clear(r: i32, g: i32, b: i32);
            pub fn window_should_close(window: Window) -> i32;
            pub fn window_keypressed(window: Window, key: i32) -> bool;

            pub fn camera_create(fov: f32, w: f32, h: f32) -> Camera;
            pub fn camera_update(cam: *mut Camera);
            pub fn camera_translate(cam: *mut Camera, translate_to: *const f32);

            pub fn fps() -> i32;
            pub fn deg2rad(deg: f32) -> f32;
            pub fn print_openglerror();
        }
    }

    #[allow(dead_code)]
    impl Mesh {
        // not in C code -- just inits everything to 0
        pub fn create() -> Self {
            Self {
                vao: 0,
                vbo: 0,
                program: 0,
                model_translation: [
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ]
                ],
                model_scale: [
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ]
                ],
                model_rotation: [
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ],
                    [ 0.0, 0.0, 0.0, 0.0 ]
                ],
            }
        }

        pub fn create_vbo(self: &mut Self, mesh_size: u32, mesh_verts: *const f32) {
            unsafe { raw_bindings::mesh_create_vbo(self, mesh_size, mesh_verts); }
        }
        pub fn create_default_vao(self: &mut Self) {
            unsafe { raw_bindings::mesh_create_default_vao(self); }
        }
        pub fn create_default_program(self: &mut Self) {
            unsafe { raw_bindings::mesh_create_default_program(self); }
        }
        pub fn draw(self: Self, vertcount: u32) {
            unsafe { raw_bindings::mesh_draw(self, vertcount); }
        }
        pub fn matrices_init(self: &mut Self) {
            unsafe { raw_bindings::mesh_matrices_init(self); }
        }
        pub fn matrices_update(self: &mut Self, cam: Camera) {
            unsafe { raw_bindings::mesh_matrices_update(self, cam); }
        }
        pub fn update(self: &mut Self, vertcount: u32, cam: Camera) {
            unsafe { raw_bindings::mesh_update(self, vertcount, cam); }
        }
    }

    #[allow(dead_code)]
    impl Window {
        pub fn create() -> Result<Window, &'static str> {
            let win: Window = unsafe { raw_bindings::window_create() };
            
            if win.window as *const c_void == std::ptr::null::<c_void>() {
                return Err("Could not create window or OpenGL context");
            }

            Ok(win)
        }
        pub fn update(self: &mut Self, cam: *mut Camera) {
            unsafe { raw_bindings::window_update(self, cam); }
        }
        pub fn swapbuffers(self: &Self) {
            unsafe { raw_bindings::window_swapbuffers(self); }
        }
        pub fn clear(r: i32, g: i32, b: i32) {
            unsafe { raw_bindings::window_clear(r, g, b); }
        }
        pub fn should_close(self: Self) -> i32 {
            unsafe { raw_bindings::window_should_close(self) }
        }
        pub fn keypressed(self: Self, key: i32) -> bool {
            unsafe { raw_bindings::window_keypressed(self, key) }
        }
    }

    #[allow(dead_code)]
    impl Camera {
        pub fn create(fov: f32, w: f32, h: f32) -> Self {
            unsafe { raw_bindings::camera_create(fov, w, h) }
        }
        pub fn update(self: &mut Self) {
            unsafe { raw_bindings::camera_update(self); }
        }
        pub fn translate(self: &mut Self, translate_to: *const f32) {
            unsafe { raw_bindings::camera_translate(self, translate_to); }
        }
    }

    pub fn deg2rad(deg: f32) -> f32 {
        unsafe { raw_bindings::deg2rad(deg) }
    }
    pub fn fps() -> i32 {
        unsafe { raw_bindings::fps() }
    }
    pub fn print_openglerror() {
        unsafe { raw_bindings::print_openglerror(); }
    }
}


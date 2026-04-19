fn main() {
    let src = "../../../src";
    cc::Build::new()
        .file(format!("{src}/var.c"))
        .file(format!("{src}/global.c"))
        .file(format!("{src}/control-flow.c"))
        .file(format!("{src}/execute.c"))
        .file(format!("{src}/warnings.c"))
        .file(format!("{src}/parsing.c"))
        .file("stubs.c")
        .include(format!("{src}/internal"))
        .define("_POSIX_C_SOURCE", "200809L")
        .compile("lo3core");
}

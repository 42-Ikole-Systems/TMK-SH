const std = @import("std");
const LineReader = @import("io/line_reader.zig");
const Lexer = @import("parser/lexer.zig");

fn s() *u8 {
    var x = 'a';
    return &x;
}

pub fn main() !void {
    var argv = std.process.args();
    _ = argv.skip();
    while (argv.next()) |arg| {
        std.debug.print("{s}\n", .{arg});
    }

    var buffered_reader = std.io.bufferedReader(std.io.getStdIn().reader());
    var reader = buffered_reader.reader();
    _ = reader;

    const allocator = std.heap.page_allocator;
    var stdin_reader = std.io.getStdIn().reader();

    var line_reader = LineReader.new(allocator, stdin_reader);

    std.debug.print("Start\n", .{});
    while (try line_reader.next()) |line| {
        std.debug.print("{s}\n", .{line});
    }

    // var lexer = Lexer.init(line_reader);
    // var parser = Parser.init(lexer);
    // while (true) {
    //     var ast: AST = parser.parse() orelse break;
    //     // todo: expander
    //     executor.execute(tree);
    //     std.debug.print("TMK: ", .{});
    //     var line = try line_reader.next();
    //     std.debug.print("{s}\n", .{line});
    // }
}

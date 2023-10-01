const std = @import("std");
const LineReader = @import("io/LineReader.zig");

pub fn main() !void {
    var argv = std.process.args();
    _ = argv.skip();
    while (argv.next()) |arg| {
        std.debug.print("{s}\n", .{arg});
    }

    const allocator = std.heap.page_allocator;

    var bufreader = std.io.bufferedReader(std.io.getStdIn().reader());
    var reader = bufreader.reader();
    var line_reader = LineReader.init(allocator, reader);

    while (try line_reader.next()) |line| {
        std.debug.print("{s}\n", .{line});
    }
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit(); // try commenting this out and see if zig detects the memory leak!
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}

test "shell tokenizer" {
    const BUFFER_SIZE = 2048;
    _ = BUFFER_SIZE;
    const allocator = std.heap.page_allocator;
    const stdin = std.io.getStdIn().reader();

    while (true) {
        const line = try stdin.readAllAlloc(allocator, 0);
        if (line.len == 0) break;
        // Do something with the line, e.g., print it
        const lineSlice = line[0 .. line.len - 1]; // Remove the trailing newline
        std.debug.print("Read line: {s}\n", .{lineSlice});

        allocator.free(line); // Release the allocated memory
    }
}

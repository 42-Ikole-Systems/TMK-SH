const std = @import("std");
// Using declarations
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;

pub fn LineReader(comptime ReaderType: type) type {
    return struct {
        buffer: ArrayList(u8),
        reader: ReaderType,

        const Self = @This();

        pub fn print(self: *Self) void {
            std.debug.print("{any}\n", .{self.buffer});
        }

        pub fn next(self: *Self) !?[]u8 {
            self.buffer.clearRetainingCapacity();
            self.reader.streamUntilDelimiter(self.buffer.writer(), '\n', null) catch |err| {
                if (err == anyerror.EndOfStream) {
                    return null;
                }
                return err;
            };
            return self.buffer.items;
        }
    };
}

pub fn init(comptime allocator: Allocator, reader: anytype) LineReader(@TypeOf(reader)) {
    var vec = ArrayList(u8).init(allocator);
    return .{ .buffer = vec, .reader = reader };
}

// https://ziglang.org/documentation/master/#toc-Pass-by-value-Parameters
pub fn getline(reader: anytype, dest: *ArrayList(u8)) !void {
    var bufferReader = std.io.bufferedReader(reader);
    try bufferReader.reader().streamUntilDelimiter(*dest.writer(), '\n');
}

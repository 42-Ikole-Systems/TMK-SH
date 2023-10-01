const std = @import("std");
// Using declarations
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const StdInReaderType = std.fs.File.Reader;
const StdInBufferedReader = std.io.BufferedReader(4096, StdInReaderType);

// pub fn LineReader(comptime ReaderType: type) type {
//     return struct {
//         buffer: ArrayList(u8),
//         reader: ReaderType,

//         const Self = @This();

//         pub fn print(self: *Self) void {
//             std.debug.print("{any}\n", .{self.buffer});
//         }

//         pub fn next(self: *Self) !?[]u8 {
//             self.buffer.clearRetainingCapacity();
//             self.reader.streamUntilDelimiter(self.buffer.writer(), '\n', null) catch |err| {
//                 if (err == anyerror.EndOfStream) {
//                     return null;
//                 }
//                 return err;
//             };
//             return self.buffer.items;
//         }
//     };
// }

const LineReader = struct {
    const Self = @This();

    buffer: ArrayList(u8),
    buffered_reader: StdInBufferedReader,

    pub fn next(self: *Self) !?[]u8 {
        self.buffer.clearRetainingCapacity();
        self.buffered_reader.reader().streamUntilDelimiter(self.buffer.writer(), '\n', null) catch |err| {
            if (err == anyerror.EndOfStream) {
                return null;
            }
            return err;
        };
        return self.buffer.items;
    }
};

pub fn new(comptime allocator: Allocator, reader: StdInReaderType) LineReader {
    var buffered_reader = std.io.bufferedReader(reader);
    var vec = ArrayList(u8).init(allocator);
    return .{ .buffer = vec, .buffered_reader = buffered_reader };
}

// pub fn init(comptime allocator: Allocator, reader: anytype) LineReader(@TypeOf(reader)) {
//     var vec = ArrayList(u8).init(allocator);
//     return .{ .buffer = vec, .reader = reader.reader() };
// }

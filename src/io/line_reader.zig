const std = @import("std");
// Using declarations
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const StdInReaderType = std.fs.File.Reader;
const StdInBufferedReader = std.io.BufferedReader(4096, StdInReaderType);

const LineReader = struct {
    const Self = @This();

    buffer: ArrayList(u8),
    reader: StdInBufferedReader.Reader,

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

pub fn new(comptime allocator: Allocator, reader: StdInBufferedReader.Reader) LineReader {
    var vec = ArrayList(u8).init(allocator);
    return .{ .buffer = vec, .reader = reader };
}

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

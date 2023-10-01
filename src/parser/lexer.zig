// const std = @import("std");
// const LineReader = @import("io/line_reader.zig");

// pub fn Lexer() type {
//     return struct {
//         const Self = @This();
//         reader: LineReader(@Type()),
//         list: ArrayList(u32),

//         pub fn nextToken(self: *Self) !?void {
//             var line: []u8 = try self.reader.next() orelse return null;
//             _ = line;
//         }
//     };
// }

// pub fn init(reader: anytype) Lexer(@Type(reader)) {
//     return .{ .reader = reader };
// }

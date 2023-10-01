// const std = @import("std");
// const LineReader = @import("io/line_reader.zig");

// pub fn Parser(comptime LexerType: type) type {
//     return struct {
//         const Self = @This();
//         lexer: LexerType,

//         pub fn nextTree(self: *Self) !?void {
//             var line: []u8 = try self.reader.nextToken() orelse return null;
//             _ = line;
//         }
//     };
// }

// pub fn init(reader: anytype) Parser(@Type(reader)) {
//     return .{ .reader = reader };
// }

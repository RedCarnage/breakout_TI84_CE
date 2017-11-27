/**
 * Copyright (c) 2017, Carl Stika
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Block playfields for the game.
 *
 * Note : Only include this in once c file.
 * max 15 bricks width.
 * max 10 bricks hieght
 * will hold.
 */
#ifndef BLOCK_PLAYFIELDS_H
#define BLOCK_PLAYFIELDS_H


struct PlayfieldInfo {
    int numRows;
    char *playfield[10];
};

#define MAX_PLAY_FIELDS 4
struct PlayfieldInfo playfields[MAX_PLAY_FIELDS] = {
                                    { 3,
                                     {
                                         "000000000000000",
                                         "011111111111110",
                                         "002222222222200"
                                     }
                                    },
                                    { 6,
                                        {
                                            "011111111111110",
                                            "000000000000000",
                                            "033333333333330",
                                            "044444444444440",
                                            "000000000000000",
                                            "066666666666660"
                                        }
                                    },
                                    { 8,
                                        {
                                            "034343434343430",
                                            "040000000000040",
                                            "030434343434030",
                                            "040300000003040",
                                            "030400000004030",
                                            "040343434343040",
                                            "030000000000030",
                                            "043434343434340"
                                        }
                                    },
                                    { 8,
                                        {
                                            "011111111111110",
                                            "022222222222220",
                                            "000000000000000",
                                            "033333333333330",
                                            "044444444444440",
                                            "000000000000000",
                                            "055555555555550",
                                            "066666666666660"
                                        }
                                    },

                                };
#endif

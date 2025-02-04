// license:BSD-3-Clause
// copyright-holders:David Haywood
/* This is the inner-most loop code (per-pixel) and thus the most performance sensitive part */

#if FLIPX == 1
#define LOOP_INCREMENTS \
			bmp++;  \
			gfx2--;
#else

#define LOOP_INCREMENTS \
			bmp++;  \
			gfx2++;
#endif


/*************** REALLY SIMPLE INNER LOOP, NON-BLENDED, NON-TINTED, SIMPLEST CASE ****************/
#if REALLY_SIMPLE == 1

#if TRANSPARENT == 1
			pen = *gfx2;
			if (pen & 0x20000000)
			{
			*bmp = pen;
#else
			*bmp = *gfx2;
#endif

/*************** REGULAR INNER LOOPS ****************/
#else // NOT REALLY_SIMPLE

			pen = *gfx2;

#if TRANSPARENT == 1
			if (pen & 0x20000000)
			{
#endif

			// convert source to clr
			pen_to_clr(pen, &s_clr.trgb);
			//s_clr.u32 = (pen >> 3); // using the union is actually significantly slower than our pen_to_clr to function!
			// source * intesity and clamp

#if TINT == 1
			s_clr.trgb.mul(*tint_clr);
#endif

			#if BLENDED == 1

				// convert destination to clr
				pen_to_clr(*bmp, &d_clr.trgb);
				//d_clr.u32 = *bmp >> 3; // using the union is actually significantly slower than our pen_to_clr to function!
				#if _SMODE == 0
				//g_profiler.start(PROFILER_USER7);

					#if _DMODE == 0
					//g_profiler.start(PROFILER_USER1);
					// this is used extensively in the games (ingame, futari title screens etc.)

					s_clr.trgb.r = colrtable_add[salpha_table[(s_clr.trgb.r)]][dalpha_table[(d_clr.trgb.r)]];
					s_clr.trgb.g = colrtable_add[salpha_table[(s_clr.trgb.g)]][dalpha_table[(d_clr.trgb.g)]];
					s_clr.trgb.b = colrtable_add[salpha_table[(s_clr.trgb.b)]][dalpha_table[(d_clr.trgb.b)]];
					#elif _DMODE == 1
					//g_profiler.start(PROFILER_USER2);
					// futari ~7%
					s_clr.trgb.r = colrtable_add[salpha_table[(s_clr.trgb.r)]][colrtable[(s_clr.trgb.r)][(d_clr.trgb.r)]];
					s_clr.trgb.g = colrtable_add[salpha_table[(s_clr.trgb.g)]][colrtable[(s_clr.trgb.g)][(d_clr.trgb.g)]];
					s_clr.trgb.b = colrtable_add[salpha_table[(s_clr.trgb.b)]][colrtable[(s_clr.trgb.b)][(d_clr.trgb.b)]];
					#elif _DMODE == 2
					//g_profiler.start(PROFILER_USER3);
					clr0.trgb.mul_fixed(s_alpha, s_clr.trgb);
					s_clr.trgb.add_with_clr_square(clr0.trgb, d_clr.trgb);
					#elif _DMODE == 3
					//g_profiler.start(PROFILER_USER4);
					clr0.trgb.mul_fixed(s_alpha, s_clr.trgb);
					s_clr.trgb.add(clr0.trgb, d_clr.trgb);

					#elif _DMODE == 4
					//g_profiler.start(PROFILER_USER5);
					clr0.trgb.mul_fixed(s_alpha, s_clr.trgb);
					s_clr.trgb.add_with_clr_mul_fixed_rev(clr0.trgb, d_alpha, d_clr.trgb);
					#elif _DMODE == 5
					// futari black character select ~13%
					//g_profiler.start(PROFILER_USER6);
					s_clr.trgb.r =  colrtable_add[salpha_table[(s_clr.trgb.r)]][colrtable_rev[(s_clr.trgb.r)][(d_clr.trgb.r)]];
					s_clr.trgb.g =  colrtable_add[salpha_table[(s_clr.trgb.g)]][colrtable_rev[(s_clr.trgb.g)][(d_clr.trgb.g)]];
					s_clr.trgb.b =  colrtable_add[salpha_table[(s_clr.trgb.b)]][colrtable_rev[(s_clr.trgb.b)][(d_clr.trgb.b)]];

					#elif _DMODE == 6
					//g_profiler.start(PROFILER_USER7);
					clr0.trgb.mul_fixed(s_alpha, s_clr.trgb);
					s_clr.trgb.add_with_clr_mul_rev_square(clr0.trgb, d_clr.trgb);
					#elif _DMODE == 7
					//g_profiler.start(PROFILER_USER8);
					clr0.trgb.mul_fixed(s_alpha, s_clr.trgb);
					s_clr.trgb.add(clr0.trgb, d_clr.trgb);
					#endif

				//g_profiler.stop();
				#elif _SMODE == 1
				//g_profiler.start(PROFILER_USER6);
				clr0.trgb.square(s_clr.trgb);

				#elif _SMODE == 2
			//  g_profiler.start(PROFILER_USER4);
					#if _DMODE == 0
					// this is used heavily on espgal2 highscore screen (~28%) optimized to avoid use of temp clr0 variable
					s_clr.trgb.r = colrtable_add[colrtable[(d_clr.trgb.r)][(s_clr.trgb.r)]][dalpha_table[(d_clr.trgb.r)]];
					s_clr.trgb.g = colrtable_add[colrtable[(d_clr.trgb.g)][(s_clr.trgb.g)]][dalpha_table[(d_clr.trgb.g)]];
					s_clr.trgb.b = colrtable_add[colrtable[(d_clr.trgb.b)][(s_clr.trgb.b)]][dalpha_table[(d_clr.trgb.b)]];
					#elif _DMODE == 1
					clr0.trgb.mul_3param(s_clr.trgb, d_clr.trgb);
					s_clr.trgb.add_with_clr_mul_3param(clr0.trgb, d_clr.trgb, s_clr.trgb);
					#elif _DMODE == 2
					clr0.trgb.mul_3param(s_clr.trgb, d_clr.trgb);
					s_clr.trgb.add_with_clr_square(clr0.trgb, d_clr.trgb);
					#elif _DMODE == 3
					clr0.trgb.mul_3param(s_clr.trgb, d_clr.trgb);
					s_clr.trgb.add(clr0.trgb, d_clr.trgb);

					#elif _DMODE == 4
					clr0.trgb.mul_3param(s_clr.trgb, d_clr.trgb);
					s_clr.trgb.add_with_clr_mul_fixed_rev(clr0.trgb, d_alpha, d_clr.trgb);
					#elif _DMODE == 5
					clr0.trgb.mul_3param(s_clr.trgb, d_clr.trgb);
					s_clr.trgb.add_with_clr_mul_rev_3param(clr0.trgb, d_clr.trgb, s_clr.trgb);
					#elif _DMODE == 6
					clr0.trgb.mul_3param(s_clr.trgb, d_clr.trgb);
					s_clr.trgb.add_with_clr_mul_rev_square(clr0.trgb, d_clr.trgb);
					#elif _DMODE == 7
					clr0.trgb.mul_3param(s_clr.trgb, d_clr.trgb);
					s_clr.trgb.add(clr0.trgb, d_clr.trgb);
					#endif
				//g_profiler.stop();

				#elif _SMODE == 3
				//g_profiler.start(PROFILER_USER1);
				clr0.trgb.copy(s_clr.trgb);

				#elif _SMODE == 4
				//g_profiler.start(PROFILER_USER2);
				clr0.trgb.mul_fixed_rev(s_alpha, s_clr.trgb);
				#elif _SMODE == 5
				//g_profiler.start(PROFILER_USER3);
				clr0.trgb.mul_rev_square(s_clr.trgb);
				#elif _SMODE == 6
				//g_profiler.start(PROFILER_USER4);
				clr0.trgb.mul_rev_3param(s_clr.trgb, d_clr.trgb);
				#elif _SMODE == 7
				//g_profiler.start(PROFILER_USER5);
				clr0.trgb.copy(s_clr.trgb);
				#endif


// smode 0/2 cases are already split up and handled above.
#if _SMODE != 2
#if _SMODE != 0

				#if _DMODE == 0
				s_clr.trgb.add_with_clr_mul_fixed(clr0.trgb, d_alpha, d_clr.trgb);
				#elif _DMODE == 1
				s_clr.trgb.add_with_clr_mul_3param(clr0.trgb, d_clr.trgb, s_clr.trgb);
				#elif _DMODE == 2
				s_clr.trgb.add_with_clr_square(clr0.trgb, d_clr.trgb);
				#elif _DMODE == 3
				s_clr.trgb.add(clr0.trgb, d_clr.trgb);

				#elif _DMODE == 4
				s_clr.trgb.add_with_clr_mul_fixed_rev(clr0.trgb, d_alpha, d_clr.trgb);
				#elif _DMODE == 5
				s_clr.trgb.add_with_clr_mul_rev_3param(clr0.trgb, d_clr.trgb, s_clr.trgb);
				#elif _DMODE == 6
				s_clr.trgb.add_with_clr_mul_rev_square(clr0.trgb, d_clr.trgb);
				#elif _DMODE == 7
				s_clr.trgb.add(clr0.trgb, d_clr.trgb);
				#endif

				//g_profiler.stop();
#endif
#endif

			#endif

			// write result
			*bmp = s_clr.trgb.to_pen() | (pen & 0x20000000);
			//*bmp = (s_clr.u32 << 3)|(pen & 0x20000000); // using the union is actually significantly slower than our to_pen function!

#endif // END NOT REALLY SIMPLE

#if TRANSPARENT == 1
			}
#endif
			LOOP_INCREMENTS
